import csv

total = 0
solved = 0

with open('crc32s.csv') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
		total = total + 1
		solved = solved + (len(row[4]) != 0)

print(str(solved) + '/' + str(total))
