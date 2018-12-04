#!/usr/bin/py
import random
import string


targetString = "lol"
generation = []
generationCount = 0
maxGenerations = False
populationPerGen = 2000

def divTwo(st):
    if(len(st)%2 == 0):
        return int(len(st)/2)
    else:
        return int((len(st)/2)+1)

def randomString(strlen):
    rtn_string = ""
    for x in range(0, strlen):
        rtn_string += random.choice(string.ascii_lowercase)
    return rtn_string
def charLen(str1, str2, inverse=False):
    if(len(str1) >= len(str2)):
        if(inverse):
            return str2
        else:
            return str1
    else:
        if(inverse):
            return str1
        else:
            return str2
def strComp(str1, str2):
    try:
        match = 0
        for char in charLen(str1, str2):
            for char2 in charLen(str1, str2, True):
                if(char == char2):
                    match += 1
        return ((match*100)/len(charLen(str1, str2)))
    except Exception as e:
        print(e)
        pass

def populate(population):
    for _ in range(0, population):
        generation.append(randomString(len(targetString)))

def breed():
    global generation
    subGen = []
    for x in generation:
        for y in generation:
            if(x != y):
                fstHalf = x[0:divTwo(x)]
                snHalf = y[divTwo(x):len(x)]

                fchild = fstHalf+snHalf
                schild = snHalf+fstHalf

                # mutation process
                if(random.random() <= .2):
                    mutatedChild = list(fchild)
                    mutatedChild[divTwo(x)] = random.choice(string.ascii_lowercase)
                    fchild = ''.join(mutatedChild)
                if(random.random() <= .15):
                    mutatedChild = list(schild)
                    mutatedChild[divTwo(x)] = random.choice(string.ascii_lowercase)
                    schild = ''.join(mutatedChild)

                subGen.append(fchild)
                subGen.append(schild)
    generation = subGen

def checkSolution():
    global generationCount
    found = False
    for x in generation:
        #print(x)
        if(x == targetString):
            found = True
            break
    if(found == False):
        print("Generation #"+str(generationCount)+" Population: "+str(len(generation)))
        breed()
        if(maxGenerations != False and maxGenerations > generationCount):
            checkSolution()
            generationCount += 1
        elif(maxGenerations == False):
            checkSolution()
            generationCount += 1
    else:
        print("Solution "+str(x)+" found at generation #"+str(generationCount))


populate(populationPerGen)
checkSolution()
