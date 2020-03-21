# kouting uses backtracking algo add all related tiles to the hands and check if it would lead to winning

#should consider have winning hand but not open hand case, 
def Is_Winning_Hand(tilesIn):

    tile_length = len(tilesIn)
    tiles_total = tilesIn
    if tile_length < 14 or tile_length > 17:
        return False

    series= [] #[[2,3,4],[7,8,9]]
    threes = []
    pairs = []

    tiles_dict = Counter(tiles)

    count_results = tiles_dict.values()
    three_count = count_results.count(3) # number of three of a kind in the hand
    pair_count = count_results.count(2) #number of pairs in the hand

    if has_gang = True: #TODO move into remove gang function

        for i in range 4:
            raw_tiles.remove(g) #TODO define g,gang
        threes.append(g) #don't seperate gang and three in the list for now


    #else: findwinninghand here

    possible_pairs = []

    for i in range(pair_counts):
        possible_pairs.append(tiles_dict.keys(2))
        
    # from here deal with general cases, start by pulling pairs