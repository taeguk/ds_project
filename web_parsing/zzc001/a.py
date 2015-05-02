
import urllib
import json
from bs4 import BeautifulSoup
url = 'http://libseat.sogang.ac.kr/seat/domian5.asp'


fp = urllib.urlopen(url)
source = fp.read()
fp.close()
source = unicode(source, 'euc-kr').encode('utf-8')

soup = BeautifulSoup(source)

#print soup.title.string

tables = soup.find_all('table')
tables = tables[1]

trs = tables.find_all('tr')
trs = trs[2:]
aws = []
for tr in trs:
	tds = tr.find_all('td')
	name = tds[1].string
	name = name.strip()
	used = tds[3].string.strip()
	if tds[2].font.font:
		tds[2].font.font.extract()
	total =  tds[2].string.strip()
	remain = tds[4].string.strip()

#for (name, total, used, reamin) in aws:
	ratio = float(used)/float(total)*100.0
	ratio = '%.2f' % ratio
	d = dict()
	d["name"] = name
	d["total"] = int(total)
	d["used"] = int(used)
	d["remain"] = int(remain)
	d["ratio"] = ratio
	aws += [d]

fp = open('../public_html/zzc001.json', 'w')
fp.write(json.dumps(aws))
fp.close()




