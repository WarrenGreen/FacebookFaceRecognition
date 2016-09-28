import requests as r
import sys
GRAPH_API_URL="https://graph.facebook.com/v2.7/"

if len(sys.argv) < 2 or sys.argv[1] == "help":
    print "usage: scraper.py <access token file>\n"
    print "outputs to index.csv\n"
    sys.exit(0)

accessTokenFile = open(sys.argv[1], "r")
ACCESS_TOKEN = accessTokenFile.readline().strip()

#Get Users ID
params={"access_token":ACCESS_TOKEN}
resp = r.get(GRAPH_API_URL+"me", params = params)
USER_ID = resp.json()["id"]

run = True
after = None
outFile = open("index.csv", "w")
pages = 0
while run and pages < 5: #hacked do while
	params={"access_token":ACCESS_TOKEN, "after":after}
	resp = r.get(GRAPH_API_URL+USER_ID+"/photos?type=tagged", params=params)
	data = resp.json()["data"]

	if "paging" in resp.json():
		after = resp.json()["paging"]["cursors"]["after"]
	else:
		run = False

	pages+=1
	for photo in data:
		photoId = photo["id"]
		tagsParams = {"access_token":ACCESS_TOKEN}
		tagsResp = r.get(GRAPH_API_URL+photoId+"/tags", params=tagsParams)
		tagsData = tagsResp.json()["data"]
		for tag in tagsData:
				if (tag["name"] == "Warren Green"):
					x = tag["x"]
					y = tag["y"]
					print str(photoId) +"," +str(x)+","+str(y)
					outFile.write("{0}, {1}, {2}\n".format(photoId, x, y))
