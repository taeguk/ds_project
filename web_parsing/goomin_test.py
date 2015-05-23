
import urllib
import json
from bs4 import BeautifulSoup
url = 'http://news.naver.com/main/read.nhn?mode=LSD&mid=shm&sid1=100&oid=023&aid=0002964568'


fp = urllib.urlopen(url)
source = fp.read()
fp.close()
source = unicode(source, 'euc-kr').encode('utf-8')

soup = BeautifulSoup(source, "html.parser")

test = soup.find("div", id="articleBodyContents")

# print test.replace('<br>', '\n')

invalid_tags = ['br', 'p', 'a']
for tag in invalid_tags:
    for match in test.findAll(tag):
        match.replaceWithChildren()
print test

