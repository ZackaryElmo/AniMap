from AnilistPython import Anilist
import csv

anilist = Anilist()

myList = anilist.search_anime(score=range(50, 99))

anilist.print_anime_info("Vinland saga")

field_names = ['name_romaji', 'name_english', 'starting_time', 'ending_time', 'cover_image', 'banner_image', 'airing_format', 'airing_status', 'airing_episodes', 'season', 'desc', 'average_score', 'genres', 'next_airing_ep']

# Open the csv file for writing
with open('AniMap.csv', 'w', newline= '', encoding='utf-8') as fileObj:
    # Create a CSV Dictwriter object
    writerObj = csv.DictWriter(fileObj, fieldnames=field_names)
   
    writerObj.writerows(myList)