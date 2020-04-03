
from collections import Counter
import copy

class Option: # each options catagotized by all possible pairs e.g. pair = [3,3] leftovers = [3,4,5,6,7,8,9,9,9]
    def __init__(self, pair, threes, series,leftovers):
        self.pair = pair
        self.threes = threes
        self.series = series
        self.leftovers = leftovers

    def __repr__(self):
        return "Pair: " + str(self.pair) + "  Sets: " + str(self.threes) + "  Sequences: " + str(self.series) +  "  Left: " + str(self.leftovers)

    def __str__(self):
        return "Pair: " + str(self.pair) + "  Sets: " + str(self.threes) + "  Sequences: " + str(self.series) +  "  Left: " + str(self.leftovers)

#please ignore the following assignments
Dragons = [100]
Ones = [1,10,20]
Nines = [9,19,29]
Tiles = [1,2,3,4,5]
Tiles_Raw = [1,2,3,4,5]
has_one_nine = False
has_gang = False



def Get_Pairs(tiles):
    pairs = []
    for t in set(tiles):
        if tiles.count(t) >= 2:
            pairs.append([t, t])
    return pairs

def Get_Threes(tiles):
    sets = []
    for t in set(tiles):
        if tiles.count(t) >= 3:
            sets.append([t, t, t])
    return sets

def Get_Series(tiles):
    # returns a list of all the sequences
    series = []
    
    tiles_no_dup = sorted(list(set(tiles)))
    temp = [[tiles_no_dup[0]]]

    for i in range (1,len(tiles_no_dup)):
        prev_tile = tiles_no_dup[i-1]

        if tiles_no_dup[i] != prev_tile+1:
            temp = [[tiles_no_dup[i]]]
            continue

        temp_series = [] 
        for t in temp:
            t.append(tiles_no_dup[i]) #adding the valid element above to the current series in progress
            if len(t) != 3:
                temp_series.append(t) #adding current element to a temporary series
            else:
                series.append(t) #once reached length 3 the temp series gets added
        temp = temp_series
        temp.append([tiles_no_dup[i]])

    return series

def Map_Remainings(patterns_found, tiles): # combos are a collection of existed pairs or sequences, 
    #this fcn removes them from the overall list, and returns the mapping of patterns and their corresponding leftovers

    collections = []
    for pattern in patterns_found:
        leftovers = copy.deepcopy(tiles)

        for p in pattern:
            leftovers.remove(p)
        collections.append((pattern, leftovers))

    return collections

def find_winning_hand(tiles):
    options = []
    winning_hand = []
    completed = []

    pairs = Get_Pairs(tiles)
    collections_by_pairs = Map_Remainings(pairs, tiles)

    #create all collections for each pulling one pair out cases
    for c in collections_by_pairs:
        options.append(Option([c[0]],[],[], c[1]))

    while len(options) > 0:
        option = options.pop(0)
        three_combos = Get_Threes(option.leftovers)
        if len(three_combos) != 0:
            possi = Map_Remainings(three_combos, option.leftovers)
            for p in possi:
                threes = copy.deepcopy(option.threes)
                threes.append(p[0])
                new_option = Option(option.pair, threes, option.series, p[1])
                if len(p[1]) == 0:
                    s_defined = copy.deepcopy(threes)
                    s_defined.extend(option.series)
                    s_defined = sorted(s_defined)
                    if not(s_defined in completed):
                        winning_hand.append(new_option)
                        completed.append(s_defined)
                    continue
                options.append(new_option)

        series_combos = Get_Series(option.leftovers)
        if len(series_combos) != 0:
            possi = Map_Remainings(series_combos, option.leftovers)
            for p in possi:
                series = copy.deepcopy(option.series)
                series.append(p[0])
                new_option = Option(option.pair, option.threes, series, p[1])
                if len(p[1]) == 0:
                    s_defined = copy.deepcopy(series)
                    s_defined.extend(option.threes)
                    s_defined = sorted(s_defined)
                    if not(s_defined in completed):
                        winning_hand.append(new_option)
                        completed.append(s_defined)
                    continue
                options.append(new_option)

    real_wins = []
    for wh in winning_hand:
        if len(wh.series) == 0 or len(wh.threes) == 0:
            continue
        real_wins.append(wh)
    return real_wins

tiles = [2,3,4,6,7,8,9,9,11,11,11] #TODO deal with gang situation removed cases
print(find_winning_hand(tiles))

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


