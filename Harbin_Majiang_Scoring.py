
from collections import Counter

Dragons = [100]
Ones = [1,10,20]
Nines = [9,19,29]
Tiles = [1,2,3,4,5]
has_one_nine = False
def Is_Winning_Hand(tilesIn):

    tile_length = len(tilesIn)

    series_list = [] #[[2,3,4],[7,8,9]]
    threes_list = []
    pairs_list = []


    if tile_length < 14 or tile_length > 17:
        return False

    tiles_total = tilesIn
    #tiles_dict = Counter(tiles)
    #counts = tiles_dict.values()

    
Is_Winning_Hand(tiles1)

def Kou_Ting_Check()
def Mo_Bao_Check()
def Series_Check(tiles):

    removed_series = []
    #use a for loop, check single tiles and remove until it's gone
    for i in len(tile_counts)

    tile_dict = Counter(tiles)
    tile_counts = tile_dict.values()
    for i in range len(tile_counts):
        if tile_counts[i] == 1:
            #tile_dict

def Build_Series(targets, tiles): 
    # targets is a list of length 1 or 2: [2] or [2,3]
    # interally generate a list of connections on both end
    # return a list with all series nested
    all_adjacents = []
    for i in len(targets):
        all_adjacents = Get_Adjacents(targets[i])

def Get_Adjacents(target):
    #takes a single target,  and return valid, sorted connections that exists in the tiles
    adjacents = []

    if target not in Tiles:
        return adjacents

    if target in Dragons:
        return adjacents

    if target in Ones:

        if target+1 in Tiles:
            adjacents.append(target+1)
    
        return adjacents

    if target in Nines:

        if target-1 in Tiles:
            adjacents.append(target-1)

        return adjacents

    if target+1 in Tiles:
        adjacents.append(target+1)

    if target-1 in Tiles:
        adjacents.append(target-1)

    return (sorted(adjacents))

def Pairs_Check(tiles)
def Threes_Check(tiles)

def One_Nine_Check(tiles):
    tile_dict = Counter(tiles)
    #if tile_dict.keys[] contains 1 or 9 tiles

# winning hand conditions should have the pattern:

# * requirement: already kou ting

# mo bao win - draw tile the same as treasure
# OR
# normal win:
            # 1. one pair
            # 2. three of a kind (or four)
            # 3. one series
            # 4. everything else is either pattern 2 or 4
            # 5. all tiles should contains at least a 1 or 9


# note that the possible winning hand only allows the number of tiles of 14 - 17 (maximum three "Gangs")

#cases to pay attention to:
    # a three of a kind might not be used as a three of a kind, it might be a pair plus an a tile that belongs to a series
    # to avoid the case above check for series first, then pick pair, then the rest should all be three of a kind (or Gang)

    # mo Bao have in complete winning hands is a win
    # 7 pairs allowed?


#======================================================================================================================
# SCORING RULES 

# Special condition - Mo bao : [12,-4,-4,-4]


# except for the special conditons above, the final score will be  calculated by using A (basic score)times B (multiplication) below

# set A - Basic conditions:
#                a. Zi mo:   [6,-2,-2,-2]
#                b. dian pao normal: [6,-4,-1,-1]
#                c. kou ting normal: [3,-1,-1,-1]


# set B - Multiplications
#                1. Hu ka (middle tile of a series) : x2
#                2. Hu bao (winning tile same as treasure) : x4

#                3. dian pao hu ka (b x 1) : [12,-8,-2,-2]
#                4. zi mo Bao (a x 2) :x8                
#                5. Zi mo Ka Bao/ bao zhong bao (a x 2 x 4): x16


#======================================================================================================================
def Calculate_Scores():
    # returns an array of four int [winner, triggerer, player1, player2] possibly [pos,neg,neg,neg]
    # scoring calculating requirements : treasure, the last tile, isWinningHand=True

    dian_pao_basic_scores = [6,-4,-1,-1]
    zi_mo_basic_scores = [6,-2,-2,-2]

    ka_bao_doubles = 2
    hu_bao_quadrupoles = 4


def Is_Hu_Ka() #winning tile is middle tile
    
def Is_Hu_Bao() #winning tile is the same as bao

def Is_Mo_Bao() # draw bao, but bao not the same as winning tile
def Hu

# kou ting chi san jia
# counter clockwise
# xia jia xian ying 
# bao is bao only when zi mo (no dian pao hu bao at the same time)
# chuang take turns, stay as chuang if win