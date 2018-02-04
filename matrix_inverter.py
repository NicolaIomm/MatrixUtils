import sys

    # Inizializza una matrice vuota di dimensione n x n OK
def init_mat(n):
    mat = []
    for i in range(n):
        mat.append([])
        for j in range(n):
            mat[i].append(0)
    return mat

    # Stampa la matrice OK
def print_mat(mat):
    n = len(mat[0])
    for i in range(n):
        for j in range(n):
            print(mat[i][j],end="\t")
        print("")

    # Calcola sottomatrice eliminando la i-esima riga e j-esima colonna OK
def calc_submat(mat, _i, _j):
    n = len(mat[0])
    newmat = []
    for i in range(n):
        if (i == _i):
            continue
        l = mat[i].copy()
        l.pop(_j)
        newmat.append(l)
    return newmat

    # Calcola determinante della matrice in modo ricorsivo OK
def calc_det(mat):
    n = len(mat[0])
    if (n == 1):
        return mat[0][0]
    else:
        det = 0
        for i in range(n):
            if (i % 2 == 0):
                det += mat[0][i] * calc_det(calc_submat(mat, 0, i))
            else:
                det -= mat[0][i] * calc_det(calc_submat(mat, 0, i))
        return det

    # Calcola matrice trasposta OK
def calc_trasp(mat):
    n = len(mat[0])
    newmat = mat.copy()
    for i in range(n):
        for j in range(n):
            if (i < j):
                tmp = newmat[i][j]
                newmat[i][j] = newmat[j][i]
                newmat[j][i] = tmp
    return newmat

    # Calcola la matrice dei complementi algebrici OK
def calc_complalg(mat):
    n = len(mat[0])
    newmat = init_mat(n)
    for i in range(n):
        for j in range(n):
            if ((i+j) % 2 == 0):
                newmat[i][j] = calc_det(calc_submat(mat, i, j))
            else:
                newmat[i][j] = -1 * calc_det(calc_submat(mat, i, j))
    return newmat

    # Calcola il prodotto di una costante per una matrice OK
def calc_moltcost(c, mat):
    n = len(mat[0])
    for i in range(n):
        for j in range(n):
            mat[i][j] = mat[i][j] * c
    return mat
    
    # Calcola matrice inversa OK
def calc_inv(mat):
    n = len(mat[0])
    det = calc_det(mat)
    print("La matrice ha determinante = %d. "%det, end="")
    if (det == 0):
        print("Non e' invertibile !")
        print("------------------------")
        sys.exit(0)
    else:
        print("\n------------------------")
        return calc_moltcost(1.0/det, calc_trasp(calc_complalg(mat)))

#########################################
#   Da qui in poi inizia il programma   #
#########################################

    # Ottieni dimensione matrice da argomento del programma OK
if (len(sys.argv) < 2):
    print("== Usage: python3 inv_mat.py [n] ==")
    print("\twhere [n] is the matrix dimension (n x n)")

n = int(sys.argv[1])
print("------------------------")
    
    # Ottieni elementi della matrice da input OK
mat = []
for i in range(n):
    line = input("Inserisci la %s-esima riga: " %i).split(" ")
    for j in range(len(line)):
        line[j] = int(line[j])
    mat.append(line)

print("------------------------")
print_mat(calc_inv(mat))
print("------------------------")