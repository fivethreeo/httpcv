import os
import logging
try:
	from urllib.request import urlopen
except ImportError:
	from urllib2 import urlopen


URL = "https://waf.io/waf-%s"
WAF_VERSION = "1.8.14"

def download(url, saveto):
	'''downloads the waf package.'''
	logging.info("wget %s" % url)
	u = None
	try:
		u = urlopen(url)
		with open(saveto, 'wb') as f: f.write(u.read())
	finally:
		if u: u.close()
	return os.path.realpath(saveto)

download(URL % WAF_VERSION, './waf')