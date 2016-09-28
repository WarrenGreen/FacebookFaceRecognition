import requests as r
import sys

GRAPH_API_URL="https://graph.facebook.com/v2.7/"

if len(sys.argv) < 3 or sys.argv[1] == "help":
	print "usage: imageDownloader.py <access token file> <output file>\n"
	print "outputs to index.csv\n"
	sys.exit(0)

accessTokenFile = open(sys.argv[1], "r")
ACCESS_TOKEN = accessTokenFile.readline().strip()
accessTokenFile.close()

OUTPUT_FILE = sys.argv[2].strip()

params={"access_token":ACCESS_TOKEN, "fields":"images"}

indexFile = open("index.csv", "r")

for line in indexFile:
	parts = line.split(",")
	photoId = parts[0].strip()
	resp = r.get(GRAPH_API_URL+photoId, params=params)
	images = resp.json()["images"]
	imgUrl = ""
	for img in images:
		if img["width"] == 720:
			imgUrl = img["source"].encode("UTF-8")

	if imgUrl:
		imgUrlSplit = imgUrl.split("?")
		imgParams = {}
		if len(imgUrlSplit) > 1:
			imgUrl = imgUrlSplit[0]
			for param in imgUrlSplit[1].split("&"):
				p = param.split("=")
				key = p[0]
				value = p[1]
				imgParams[key] = value
		imageResp = r.get(imgUrl, params=imgParams);
		with open(OUTPUT_FILE+photoId+".jpg", "wb") as imageFile:
			imageResp.raw.decode_content = True
			imageFile.write(imageResp.content)




indexFile.close()
