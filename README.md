# 🎮 2CCPP – Tetris-Like Strategy Game (C++ Project)

Projet réalisé dans le cadre de la 2e année CCPP — développement d’un jeu de stratégie basé sur la pose de tuiles, les bonus et la capture de zones.

Ce jeu se joue de **2 à 9 joueurs**, chacun possédant une couleur, des tuiles aléatoires et différents bonus (échange, pierre, furtivité).  
Un système avancé de grille, de capture de bonus et de scoring final permet de déterminer le vainqueur.

---

## 👥 **Collaborateurs**
- **Thomas-Martin2512 | email: thomas.martin@supinfo.com**  
- **Ay0ko | email: simon.mennechet@supinfo.com**

---

## 📘 **Documentation**

### 📖 Documentation technique (Doxygen)
La documentation complète du code C++ est générée avec **Doxygen**.

➡️ **Disponible dans :**
Cette documentation inclut :
- Diagrammes de classes
- Documentation des fonctions (paramètres, comportements…)
- Documentation des modules (Board, Game, Tile, Bonus, Player…)
- Architecture générale du projet

---

### 📚 Documentation utilisateur
Un PDF dédié à l’utilisation du jeu est disponible ici :

➡️ **User Documentation/Documentation Utilisateur 2CCPP.pdf**

Cette documentation explique :
- Comment lancer le jeu
- Les règles complètes
- Les bonus (Exchange, Rock)
- Le système de capture
- Le déroulement d’un tour
- Le scoring final

---

## 🗂️ Structure du projet
```
2CCPP_Project/
│
├── docs/
│ ├── latex/ # Documentation technique Doxygen
│ │ └── refman.pdf
│ └── user/ # Documentation utilisateur
│ └── manuel_utilisateur.pdf
│
├── include/
│ ├── Board/
│ ├── Bonus/
│ ├── Display_Board/
│ ├── Game/
│ ├── Player/
│ └── Tile/
│
├── src/
│ ├── Board/
│ ├── Bonus/
│ ├── Display_Board/
│ ├── Game/
│ ├── Player/
│ └── Tile/
│
├── Shapes.json # Définitions des tuiles disponibles
├── CMakeLists.txt # Compilation (optionnel)
└── main.cpp # Point d'entrée du programme
```
---

## ⚙️ **Compilation**

### 🔧 Compilation avec g++ (Linux / MinGW)
```bash
g++ -std=c++17 -Iinclude -o game main.cpp src/**/*.cpp
````

🛠️ Compilation avec CMake
```bash
mkdir build
cd build
cmake ..
make
./game
```

## ▶️ ** Lancer le jeu **

Une fois compilé :
```bash
./game
```

---

## 🎯 ** Fonctionnalités principales **

Placement de tuiles selon rotation et flip

Système de capture pour les bonus :

E → Exchange coupon

R → Rock (poser un rocher obligatoire)

Gestion des couleurs avec ANSI (compatible Linux & Windows)

File d’attente de tuiles avec échange

Phase finale 1x1

Calcul final :

Plus grand carré

Nombre total de cases

Détermination automatique du gagnant

---

## 📩 ** Issues / Suggestions **

Les rapports de bugs, améliorations ou idées sont les bienvenus via la section Issues du dépôt.

---

## 🏆 ** Merci d'utiliser notre jeu ! **

N’hésitez pas à cloner, tester, et modifier le projet.
Bonne partie 🎲 !
