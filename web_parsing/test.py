import urllib
import json
from bs4 import BeautifulSoup

url = 'http://news.naver.com/main/read.nhn?mode=LSD&mid=shm&sid1=100&oid=025&aid=0002474098'

fp = urllib.urlopen(url)
source = fp.read()
fp.close()
source = unicode(source, 'euc-kr').encode('utf-8')

soup = BeautifulSoup(source, "html.parser")

#print source

print soup.find('div', {'id' : 'articleBodyContents'}).contents

