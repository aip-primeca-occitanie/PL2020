import numpy as np
import sys
from importlib import import_module

# Etapes dans ce programme:
# 1 - LIRE LE CONFIG
# 2 - LIRE LE LOG
# 3 - COMPARER LOG ET CONFIG
# 4 - AFFICHER INFOS FINALS

# On considère que:
# - Il y a max 6 produits
# - Il y a 8 taches possible, correspondant aux 8 postes
# - Chaque produit peut subir 3 tâches maximum

test = 1 # Cette variable indique s'il y a une erreur ou non: 1 - PAS D'ERREUR  /  0 - ERREUR

# --------------- 1 - LIRE LE CONFIG ---------------



mon_config = open("ProductConfiguration.config","r")
while(1):
    contenu_ligne=mon_config.readline()
    if "Start" in contenu_ligne :
        break

contenu_ligne=mon_config.readline() # Enregistre la ligne dans contenu_ligne
nbre_shuttle = contenu_ligne.split(":") # Split la ligne en tableau, séparation des ":"
nbre_shuttle = int(nbre_shuttle[1]) # le tableau précédement est de type str, on le met de type int
#print(nbre_shuttle) # on affiche le nombre de shuttle
#print(type(nbre_shuttle)) # on affiche le type, pour vérifier que ce soit bien int
# --> nbre_shuttle avec split est de type "list", mais chaque élément de cette liste est de type "str"

contenu=mon_config.readlines()
# contenu est de type "list" et contient toutes les lignes decrivant les produit du fichier .config
mon_config.close()

produit = [] # contient le nom de chaque produit
tache = [] # contient les destinations que chaque produit
dure = [] # contient les durées de chaque tache
nb_produit = [] # contient le nombre de fois qu'un même produit doit être créer

verif_config = [] # sert juste a créer temps
temps = [] # contient la durée des taches

production = []


for i in range(6):
        temps.append([0, 0, 0, 0, 0, 0, 0, 0]) # Initialisation de la matrice temps 6x8 ( car 6 produits et 8 postes (ou taches) )


for ligne in contenu : # on travail ligne par ligne, donc produit par produit

    conf=ligne.split(":") # on split la ligne en séparant les strings (ou str) des ":"

    # NOM PRODUIT
    nom = conf[0].replace(" ","") # on enlève les espace a conf[0] (nom du produit) si l'etudiant a mis des espaces
    produit.append(nom) # on ajoute le nom du produit dans la list produit. On aura donc juste le nom du produit et pas d'espace
    if nom == "A" : produit_num=1
    if nom == "B" : produit_num=2
    if nom == "C" : produit_num=3
    if nom == "D" : produit_num=4
    if nom == "E" : produit_num=5
    if nom == "F" : produit_num=6

    # DESTINATIONS (ou taches) ET production
    tache_var = conf[1].replace(" ","") # enlève les espaces
    tache_var = list(tache_var) # sépare chaque destination, mais en str
    taille = len(tache_var) # regade taille de var, donc nombre de destinations
    for i in range(taille):
        tache_var[i] = int(tache_var[i]) # on converti chaque destination de str en int
    tache.append(tache_var)
    if taille == 1:
        production.append([produit_num, tache_var[0], 0, 0])
    elif taille == 2:
        production.append([produit_num, tache_var[0], tache_var[1], 0])
    elif taille == 3:
        production.append([produit_num, tache_var[0], tache_var[1], tache_var[2]])

    # DUREE
    dure_var = conf[2].split(" ")
    taille = len(dure_var)
    del dure_var[0]
    del dure_var[taille-2]
    taille = len(dure_var) # regade taille de var, donc nombre de destinations
    for i in range(taille):
        dure_var[i] = int(dure_var[i]) # on converti chaque destination de str en int
    dure.append(dure_var)

    # NOMBRE PRODUIT
    var = int(conf[3])
    nb_produit.append(var)


    # REMPLIR TABLEAU verif_config
    taille = len(tache_var)
    for i in range(taille):
        verif_config.append([produit_num, tache_var[i], dure_var[i]])

    # FIN de lire ligne par ligne


# REMPLIR TABLEAU temps
taille = len(verif_config)
for i in range(taille):
    temps[verif_config[i][0]-1][verif_config[i][1]-1] = verif_config[i][2]



# RECAP: Donc maintenant on a: temps / nb_produit / production (ET verif_config mais ne servira pas a comparer)



# --------------- 2 - LIRE LE LOG ---------------



verif_config_log = [] # sert juste a créer temps_log
produit_final = []
temps_log = []
taches_time = [] # contient le temps où les taches sont effectuer (utile pour afficher le temps de simulation où il y a une erreur)
nb_produit_log = []

produit_entree = [] # Affichera à la suite de la lecture du log les produits qui apparaisent sur les postes produit_entree[i] = [P, time]; donc ce tableau est dans l'odre chronologique d'apparition des produits
produit_duree = [] # Contient la durée totale de présence des produits dans la simulation. A la fin on divisera cette somme par le nombre de produit pour avoir une moyenne de temps de présence de chaque produit
for i in range(6): # Pour sommer les durée ont les initialise à 0, 6 ligne pour chaque produit
    produit_duree.append(0.0)

for i in range(6):
    nb_produit_log.append(0)


for i in range(6):
    produit_final.append([0, 0, 0, 0]) # Ce tableau aura la forme produit_final[j] = [P, D1, D2, D3]
    temps_log.append([0, 0, 0, 0, 0, 0, 0, 0]) # Initialisation de la matrice temps_log 6x8 ( car 6 produits et 8 postes (ou taches) )
    taches_time.append([0, 0, 0, 0, 0, 0, 0, 0]) # Initialisation de la matrice comme temps_log


Log_file = open("ModelLog.txt","r") # voir si on peut créer une variable strg qui contient le contenu de Log comme ca on peut tout de suite fermer le Log
Log=Log_file.readlines()
Log_file.close()

for line in Log :
    info=line.replace(" ","") #on enleve tous les espaces de la ligne
    info=info.split(":") #on decoupe on fonction de ":"
    ID=info[0] #le premier terme est l'identifiant ID

    if ID == "OperationPosteVide":
        poste = int(info[1])
        time = float(info[2].strip('\n'))
        print ('ERREUR à t={}s: operation sur poste vide numero {} '.format(time, poste))
        test = 0

    if ID == "EvacuationVide":
        time = float(info[1].strip('\n'))
        print ('ERREUR à t={}s: il y a une evacuation vide au poste 4'.format(time))
        test = 0

    if ID == "EcrasementProduit":
        poste = int(info[1])
        produit = int(info[2])
        time = float(info[3].strip('\n'))
        print ('ERREUR à t={}s: il y a un produit {} écrasé au poste {} '.format(time,produit,poste))
        test = 0

    if ID == "TempoT" : #identification de la ligne de Log
        P=int(info[1])
        D=int(info[2])
        T=float(info[3])
        time = float(info[4].strip('\n')) #car c est le dernier element du tableau
        #print ('Le temps T= {2} s de la tache {1} sur le produit {0}'.format(P,D,T))
        verif_config_log.append([P, D, T, time])

    if ID == "Sortie" :
        P=int(info[1])
        D1=int(info[2])
        D2=int(info[3])
        D3=int(info[4])
        time = float(info[5].strip('\n'))
        #print ('Le produit {0} est sortie avec l empilement {1},{2},{3}'.format(P,D1,D2,D3))
        produit_final = [P, D1, D2, D3] # on ecrasera cette ligne du tableau apres le test
        nb_produit_log[P-1] = nb_produit_log[P-1] + 1
        # durée de vie du produit
        k = 0 # variable qui compte à quelle ligne dans produit_entree se trouve le plus ancien produit apparu
        while(1): # On considère que c'est impossible d'avoir dans le log plus de "Sortie" que de "NewProduct". Il n'est donc pas nécessaire d'afficher une erreur sur ça car quoique fasse l'étudiant il y aura forcément plus ou le même nombre de NewProduct que de Sortie
            if produit_entree[k][0] == P: # lorsqu'on a trouver la ligne où se trouve le plus ancien produit apparu
                produit_duree[P-1] = produit_duree[P-1] + (time - produit_entree[k][1]) # Somme du temps de présence du produit P
                produit_entree[k][0] = 0.0 # Comme le produit est sortie on initialise sa ligne dans produit_entree à 0 comme ça elle ne pourra plus servir à calculer produit_duree
                break
            else:
                k = k+1

        # Vérifie si produit sortie est dans le bon ordre
        if produit_final != production[P-1]:
            test = 0
            print('ERREUR à t={}s: enchainement taches de la version {} du produit {} doit être {} et pas {}'.format(time,nb_produit_log[P-1],P,production[P-1][1:4],produit_final[1:4]))

    if ID == "NewProduct":
        P = int(info[1])
        time = float(info[2].strip('\n'))
        produit_entree.append([P, time]) # A chaque fois qu'un produit apparait sur un post on l'ajoute dans le tableau en indiquant le temps d'apparition. Ce tableau servira ensuite a calculer le temps de présence du produit jusqu'ç ce qu'il sorte


# REMPLIR TABLEAU temps_log
taille = len(verif_config_log)
for i in range(taille):
    temps_log[verif_config_log[i][0]-1][verif_config_log[i][1]-1] = verif_config_log[i][2]
    taches_time[verif_config_log[i][0]-1][verif_config_log[i][1]-1] = verif_config_log[i][3]

for i in range(6):
    if nb_produit_log[i] != 0:
        produit_duree[i] = produit_duree[i]/nb_produit_log[i] # On calcule le temps de présence moyen pour chaque produit (comme il peut y avoir plusieurs fois le même produit)



# RECAP: Donc maintenant on a: temps_log / produit_final / nb_produit_log (ET verif_config_log mais ne servira pas à comparer)



# --------------- 3 - COMPARAISON ENTRE LOG ET CONFIG ---------------



# 3 trucs a verifier:
#   - temps et temps_log via verif_temps: vérifie que le temps de chaque actions et bon ET que chaque actions a été réaliser (actions = taches)
#   - production et produit_final: vérifie la bon ordre des tâches de chaque produit --> SE FAIT DANS PARTI 2 !
#   - nb_produit et nb_produit_log: vérifie que le bon nombre de chaque produit et bien sortis



# Initialisation de verif_temps
verif_temps = []
for i in range(6):
        verif_temps.append([0, 0, 0, 0, 0, 0, 0, 0]) # Pour définir la taille de verif_temp, qui a la meme forme que temps et temps_log mais ne contient pas de temps mais du booléen (0 ou 1)
for i in range(6):
    for j in range(8):
        if temps[i][j] == 0:
            verif_temps[i][j] = 1
        else:
            verif_temps[i][j] = 0 # si on met pas ce else alors tout ce met à 1 à partir du premier 0 détecter (on sait pas expliquer pourquoi, bizarre)

# Comparer temps et temps_log, en complétant verif_temps à 1 si temps respecter, sinon reste à 0, on fait comme ça pour par la suite connaitre exactement quel temps est incorrecte
for i in range(6):
    for j in range(8):
        if temps[i][j] >= temps_log[i][j]:
            verif_temps[i][j] = 1

# Si au moins une cellule de verif_temps est à 0, ça signifie qu'un temps n'a pas été respecté et donc test = 0 (erreur)
for i in range(6):
    for j in range(8):
        if verif_temps[i][j] == 0:
            test = 0
            print('ERREUR à t={}s: durée tache {} du produit {} doit être <= à {}s, ici durée = {}s'.format(taches_time[i][j],j+1,i+1,temps[i][j],temps_log[i][j]))

# Compare nb_produit et nb_produit_log pour voir si le bon nombre de produit est bien sorti
for i in range(6):
    if nb_produit[i] != nb_produit_log[i]:
        test = 0
        print('ERREUR: le produit {} doit être crée {} fois or vous l avez créer {} fois'.format(i+1,nb_produit[i],nb_produit_log[i]))



# --------------- 4 - AFFICHER INFOS FINALS ---------------



# S'affiche seulement s'il n'y a aucune erreur donc si test = 1

if test == 1:
    for i in range(6):
        print('Le produit {} a été assemblé {} fois avec l empilement de tâche {}'.format(i+1, nb_produit_log[i], production[i][1:4]))
        print('Le temps moyen d assemblage de ce produit est de {}s'.format(produit_duree[i]))
        print(' ')





# ---- PRINT RESULTAT ------

#print(produit); print(" ")
#print(tache); print(" ")
#print(dure); print(" ")
#print(production); print(" ")
#print(nb_produit); print(" ")
#print(verif_config); print(" ")
#print(temps); print(" ")

#print(verif_config_log); print(" ")
#print(produit_final); print(" ")
#print(temps_log); print(" ")
#print(nb_produit_log); print(" ")
#print(produit_entree); print(" ")
#print(produit_duree); print(" ")

#print(verif_temps); print(" ")
#print(test); print(" ")
