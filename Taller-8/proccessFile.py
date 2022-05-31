def readFile(nombre):
    with open(nombre, 'r') as tsp:
        file =  tsp.readlines()
        positionDisplayDataSection = file.index('NODE_COORD_SECTION\n')
        displayDataSection = file[positionDisplayDataSection+1:]
        return displayDataSection;

def createVCordenadas(coordenadas):
    coordenadasX = []
    coordenadasY = []
    for elem in coordenadas[:len(coordenadas)-1]:
        coordenada = elem.split(' ')[1:]
        newCoordenada = [x for x in coordenada if x != '']
        coordenadasX.append(newCoordenada[1])
        coordenadasY.append(newCoordenada[2].split('\n')[0])
    return [coordenadasX, coordenadasY]

def writeFile(nombre, vector):
    f = open(nombre, mode="w")
    for i in vector:
        f.write(f"{i}\n")
    f.close()

if __name__ == "__main__":
    coordenadas = readFile("att48.tsp")
    [coorX, coorY] = createVCordenadas(coordenadas)
    writeFile("coordenadasX.txt", coorX)
    writeFile("coordenadasY.txt", coorY)
