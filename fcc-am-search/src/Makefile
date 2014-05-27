
active: HD.dat
	awk -F'|' '$$6 == "A" {print $$5}' < HD.dat > active
	awk -F'|' '$$6 == "E" {print $$5}' < HD.dat > expired
	awk -F'|' '$$6 == "C" {print $$5}' < HD.dat > cancelled
	awk -F'|' '$$6 == "T" {print $$5}' < HD.dat > termintated

download:
	curl -o l_amat.zip http://wireless.fcc.gov/uls/data/complete/l_amat.zip
	unzip -o l_amat

