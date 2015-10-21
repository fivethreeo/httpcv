#include "Simple-Web-Server/server_http.hpp"

#include "vmime/vmime.hpp"

//Added for the json-example
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//Added for the default_resource example
#include <fstream>
#include <vector>
#include <numeric>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


using namespace std;
using namespace cv;
//Added for the json-example:
using namespace boost::property_tree;


vmime::byteArray image_data;
bool data_changed = false;
void sortCorners(std::vector<cv::Point>& corners, cv::Point center)
{
    std::vector<cv::Point> top, bot;

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }

    cv::Point tl = top[0].x > top[1].x ? top[1] : top[0];
    cv::Point tr = top[0].x > top[1].x ? top[0] : top[1];
    cv::Point bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    cv::Point br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
}

// comparison function object
bool compareContourAreas ( std::vector<Point> contour1, std::vector<Point> contour2 ) {
    double i = fabs( contourArea(Mat(contour1)) );
    double j = fabs( contourArea(Mat(contour2)) );
    return ( i > j );
}

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
int main() {
    //HTTP-server at port 8080 using 1 threads
    HttpServer server(8080, 1, 50);
    
    //Add resources using path-regex and method-string, and an anonymous function
    //POST-example for the path /string, responds the posted string
    server.resource["^/string$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        //Retrieve string:
        auto content=request->content.string();
        //request->content.string() is a convenience function for:
        //stringstream ss;
        //ss << request->content.rdbuf();
        //string content=ss.str();
        
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
    
    //POST-example for the path /json, responds firstName+" "+lastName from the posted json
    //Responds with an appropriate error message if the posted json is not valid, or if firstName or lastName is missing
    //Example posted json:
    //{
    //  "firstName": "John",
    //  "lastName": "Smith",
    //  "age": 25
    //}
    server.resource["^/json$"]["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        try {
            ptree pt;
            read_json(request->content, pt);

            string name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");

            response << "HTTP/1.1 200 OK\r\nContent-Length: " << name.length() << "\r\n\r\n" << name;
        }
        catch(exception& e) {
            response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
    };
    
    //GET-example for the path /info
    //Responds with request-information
    server.resource["^/info$"]["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        stringstream content_stream;
        content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
        content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
        for(auto& header: request->header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }
        
        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);
        
        response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
    };
    
    //GET-example for the path /match/[number], responds with the matched string in path (number)
    //For instance a request GET /match/123 will receive: 123
    server.resource["^/match/([0-9]+)$"]["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        string number=request->path_match[1];
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << number.length() << "\r\n\r\n" << number;
    };
    
    //Default GET-example. If no other matches, this anonymous function will be called. 
    //Will respond with content in the web/-directory, and its subdirectories.
    //Default file: index.html
    //Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
    server.default_resource["GET"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        boost::filesystem::path web_root_path("web");
        if(!boost::filesystem::exists(web_root_path))
            cerr << "Could not find web root." << endl;
        else {
            auto path=web_root_path;
            path+=request->path;
            if(boost::filesystem::exists(path)) {
                if(boost::filesystem::canonical(web_root_path)<=boost::filesystem::canonical(path)) {
                    if(boost::filesystem::is_directory(path))
                        path+="/index.html";
                    if(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)) {
                        ifstream ifs;
                        ifs.open(path.string(), ifstream::in | ios::binary);
                        
                        if(ifs) {
                            ifs.seekg(0, ios::end);
                            size_t length=ifs.tellg();
                            
                            ifs.seekg(0, ios::beg);
                            
                            response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
                            
                            //read and send 128 KB at a time
                            size_t buffer_size=131072;
                            vector<char> buffer;
                            buffer.reserve(buffer_size);
                            size_t read_length;
                            try {
                                while((read_length=ifs.read(&buffer[0], buffer_size).gcount())>0) {
                                    response.write(&buffer[0], read_length);
                                    response.flush();
                                }
                            }
                            catch(const exception &e) {
                                cerr << "Connection interrupted, closing file" << endl;
                            }

                            ifs.close();
                            return;
                        }
                    }
                }
            }
        }
        string content="Could not open path "+request->path;
        response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
    server.default_resource["POST"]=[](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
        string message="File received";
        vmime::utility::inputStreamAdapter is(request->content);
        vmime::string stringdata;
        vmime::utility::outputStreamStringAdapter os(stringdata);
        vmime::utility::bufferedStreamCopy(is , os);
        vmime::shared_ptr<vmime::message> msg = vmime::make_shared<vmime::message>();
        const auto it=request->header.find("Content-Type");
        msg->parse(stringdata);       

        try
        {
            vmime::messageParser mp(msg);
            // Output information about attachments
            cout << "Message has " << mp.getAttachmentCount() << "attachment(s)" << endl;
            for(int i=0;i<mp.getAttachmentCount();++i)
            {
                vmime::shared_ptr <const vmime::attachment> att = mp.getAttachmentAt(i);
                cerr << " - " << att->getType().generate() << endl;
                image_data.clear();
                vmime::utility::outputStreamByteArrayAdapter adapter(image_data);
                att->getData()->extract(adapter);
                data_changed = true;
            }
        }
        catch(...)
    {
            cerr << boost::current_exception_diagnostic_information() << std::endl;

    }
        response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;
                
    };
    boost::thread server_thread([&server](){
        //Start server
        server.start();
    });
    boost::thread display_thread([](){
        for (;;)
        {
          try
           {
              boost::this_thread::interruption_point();
           }
           catch(boost::thread_interrupted&)
           {
              break;
           }
           if (data_changed) {
              data_changed = false;
              try
              {
                  Mat matrix = imdecode(image_data, 1);
                  Mat output;
                  Mat resized;          
                  Mat gray;
                  Mat blurred;
                  Mat canny;
                  std::vector<std::vector<Point> > contours;
                  std::vector<Point> srcpoints;

                  double rescalefactor = (double)(600)/matrix.size().width;
                  resize(matrix, resized, Size(), rescalefactor, rescalefactor);
                  cvtColor(resized, gray, COLOR_BGR2GRAY);
                  GaussianBlur(gray, blurred, Size(7, 7), 1);
                  Canny(blurred, canny, 75, 200);
                  findContours(canny, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
                  std::sort(contours.begin(), contours.end(), compareContourAreas);
                  for(size_t i=0; i<contours.size(); i++) {
                       // use contours[i] for the current contour
                       double arclen = arcLength(contours[i], true);
                       approxPolyDP(contours[i], srcpoints, 0.02*arclen, true);   
                       if (srcpoints.size()==4) {
                           break;
                       }
                  }
                  std::vector<std::vector<Point> > draw_contours;
                  draw_contours.push_back(srcpoints);
                  drawContours(resized, contours, -1, Scalar(0, 255, 0));
                  drawContours(resized, draw_contours, -1, Scalar(255, 0, 0));
                  Rect bRect = boundingRect(srcpoints);
                  rectangle(resized, 
                    Point(bRect.x, bRect.y),
                    Point(bRect.x+bRect.width, bRect.y+bRect.height),
                    Scalar(0, 0, 255)
                  );
                  Moments M = moments(srcpoints);
                  Point centroid(int(M.m10/M.m00), int(M.m01/M.m00));
                  sortCorners(srcpoints, centroid);
                  
                  vector<Point2f> srcfpoints(4);
                  Mat(srcpoints).convertTo(srcfpoints, Mat(srcfpoints).type());
                  for(size_t i=0; i<srcfpoints.size(); i++) {
                    srcfpoints[i] *= 1./rescalefactor;
                  }
                  auto maxWidth = max(
                    sqrt(pow(srcfpoints[0].x-srcfpoints[1].x,2) + pow(srcfpoints[1].x-srcfpoints[0].x,2)),
                    sqrt(pow(srcfpoints[2].x-srcfpoints[3].x,2) + pow(srcfpoints[3].x-srcfpoints[2].x,2))
                  );

                  auto maxHeight= max(
                    sqrt(pow(srcfpoints[0].y-srcfpoints[3].y,2) + pow(srcfpoints[3].y-srcfpoints[0].y,2)),
                    sqrt(pow(srcfpoints[1].y-srcfpoints[2].y,2) + pow(srcfpoints[2].y-srcfpoints[1].y,2))
                  );
                  vector<Point2f> dstpoints(4);
                  dstpoints[0] = Point2f(0,0);
                  dstpoints[1] = Point2f(maxWidth - 1, 0);
                  dstpoints[2] = Point2f(maxWidth - 1, maxHeight - 1);
                  dstpoints[3] = Point2f(0, maxHeight - 1);
                  Mat lambda = getPerspectiveTransform(srcfpoints, dstpoints);
                  warpPerspective(matrix, output, lambda, Size(maxWidth, maxHeight));
                  imshow("OImage", output); /**/ /**/
              }
              catch(const Exception& e)
              {
                  const char* err_msg = e.what();
                  cerr << "exception caught: " << err_msg << std::endl;
              }
              catch(const exception &e)
              {
                  const char* err_msg = e.what();
                  cerr << "exception caught: " << err_msg << std::endl;
              }

           }
           waitKey(100);
        }
    });
    
    
    //Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    server_thread.join();
    
    display_thread.interrupt();
    display_thread.join();

    return 0;
}
