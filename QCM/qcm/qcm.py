from flask import (
    current_app
)
from random import *
import os
import pyscan


def est_groupe(L):
    return len(L) > 3


def txt2qcm(fichier):
    qcm = []  # qcm = [[Q1, [R1, R2, R3 ...], C], ... , [debut_groupe, [[Q,[]], ...  ], fin_groupe] ... ]
    groupe = False  # Groupe de question
    etat = 'I'  # etat initial, en attente d'une question ou d un groupe
    with open(fichier, 'r', encoding='utf8') as f:
        ligne = f.readline().lstrip()
        while ligne:
            if etat == 'I':
                if ligne[:2] == '*(':
                    deb_groupe = ligne[2:]
                    ligne = f.readline().lstrip()
                    while ligne and ligne[:1] != '*':
                        deb_groupe += ligne
                        ligne = f.readline().lstrip()
                    groupe = [deb_groupe]  # prend ainsi la valeur True
                    etat = 'Q'
                elif ligne[:1] == '*':
                    etat = 'Q'
                else:
                    raise ('Fichier non conforme')
            elif etat == 'Q':
                question = ligne[1:]
                ligne = f.readline().lstrip()
                while ligne and ligne[:1] not in '+-':
                    question += ligne
                    ligne = f.readline().lstrip()
                etat = 'R'
                liste_reponse = []
            elif etat == 'R':
                while ligne and ligne[:1] in '+-':
                    reponse = ligne[1:-1] + ' '
                    if ligne[:1] == '+':
                        statut = True
                    else:
                        statut = False
                    ligne = f.readline().lstrip()
                    while ligne and ligne[:1] not in '+-*?':
                        reponse += ligne
                        ligne = f.readline().lstrip()
                    liste_reponse.append((statut, reponse))
                    if ligne[:1] == '?':
                        correction = ligne[1:]
                        ligne = f.readline().lstrip()
                        while ligne and ligne[:1] != '*':
                            correction += ligne
                            ligne = f.readline().lstrip()
                    else:
                        correction = ''
                if groupe and ligne[:2] == '*)':
                    groupe.append([question, liste_reponse, correction])
                    fin_groupe = ligne[2:]
                    ligne = f.readline().lstrip()
                    while ligne and ligne[:1] != '*':
                        fin_groupe += ligne
                        ligne = f.readline().lstrip()
                    groupe.append(fin_groupe)
                    etat = 'I'
                    qcm.append(groupe)
                    groupe = False
                elif groupe and ligne[:1] == '*':
                    groupe.append([question, liste_reponse, correction])
                    etat = 'Q'
                else:
                    qcm.append([question, liste_reponse, correction])
                    etat = 'I'
    return qcm


def notation(fsol, felv, listelv, fres):
    lsol = []
    with open(fsol, 'r', encoding='utf8') as f:
        line = f.readline()
        lsol.append(str.split(line, ';')[1:20 + 1])
        while line:
            line = f.readline()
            lsol.append(str.split(line, ';')[1:20 + 1])

    lelv = []
    with open(felv, 'r', encoding='utf8') as f:
        line = f.readline()
        lelv.append(str.split(line, ';')[1:20 + 1])
        while line:
            line = f.readline()
            lelv.append(str.split(line, ';')[1:20 + 1])

    lnotes = []
    for i in range(0, len(listelv)):
        if lelv[i][0] == '-':
            lnotes.append([listelv[i], '-'])
        else:
            note_i = 0
            for j in range(0, 20):
                if lelv[i][j] == lsol[i][j]:
                    note_i += 1
            lnotes.append([listelv[i], note_i])

    with open(fres, 'w', encoding='latin1') as f:
        for i in range(0, len(listelv)):
            f.write(lnotes[i][0][0] + ' ' + lnotes[i][0][1] + ' ' + str(lnotes[i][1]) + '\n')

    return


def qcm2tex(qcm, n, path, eleves, sol=False, cor=False):
    lRep = []
    with open(os.path.join(path, 'sujet.tex'), 'w', encoding='latin1') as f:
        with open(current_app.config['LATEX_HOME'] + 'entete2.tex', 'r', encoding='utf8') as e:
            line = e.readline()
            while line:
                f.write(line)
                line = e.readline()
        f.write('\\newcommand{\\sol}[1]{')
        if sol:
            f.write('\\fbox{#1}}\n')
        else:
            f.write('#1}\n')
        f.write('\\newcommand{\\corr}[1]{')
        if cor:
            f.write('\paragraph{Correction}#1}\n')
        else:
            f.write('}\n')

        f.write('\\begin{document}\n')

        for i in range(1, n + 1):
            cpt = i
            lRep.append([])
            compteur = 0
            seed(i)
            ordre = [i for i in range(len(qcm))]
            shuffle(ordre)
            numCopie = str(i).rjust(2, '0')

            if i > 1:
                f.write('\\newpage\n')

            f.write('\\dotfill Mathématiques\\dotfill Evaluation, durée : $1$h   \\dotfill 2019-2020.\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\hrule\n')
            f.write('\\vskip0.2cm\n')
            f.write(
                'Pour chacune de ces questions, il n\'y a qu\'une seule bonne réponse. Une absence de réponse donne 0 point, une bonne réponse rapporte 1 point.\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\hrule\n')
            f.write('\\vskip0.2cm\n')

            f.write('NUMERO COPIE : ' + numCopie + ' \\hspace{\\stretch{2}} Nom : ' + eleves[i - 1][
                0] + '\\hspace{\\stretch{1}} Prénom : ' + eleves[i - 1][1] + '\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\hrule\n')
            f.write('\\begin{enumerate}\n')
            shuffle(qcm)
            for i in ordre:
                #                if compteur > 15 and nb_page > 1:
                #                    f.write('\\eject\n N°~copie \dotfill\n')
                #                    nb_page -= 1
                L = qcm[i]
                f.write('\\item ' + L[0])
                reponses = L[1][:]
                compteur += 1
                shuffle(reponses)
                nr = len(reponses)
                idRep = 1
                f.write('\\begin{multicols}{' + str(nr) + '}\n')
                f.write('\\begin{enumerate}\n')
                for (b, r) in reponses:
                    if b:
                        r = '\\sol{' + r + '}'
                        if idRep == 1:
                            lRep[cpt - 1].append('a')
                        elif idRep == 2:
                            lRep[cpt - 1].append('b')
                        elif idRep == 3:
                            lRep[cpt - 1].append('c')
                        elif idRep == 4:
                            lRep[cpt - 1].append('d')
                        elif idRep == 5:
                            lRep[cpt - 1].append('e')
                        else:
                            lRep[cpt - 1].append('?')
                    f.write('\\item{' + r + '} \n')
                    idRep += 1
                f.write('\\end{enumerate}\n')
                f.write('\\end{multicols}\n')
                correction = L[2]
                f.write('\\corr{' + correction + '}\n')
            f.write('\\end{enumerate}\n')

        with open(current_app.config['LATEX_HOME'] + 'fin2.tex', 'r', encoding='utf8') as e:
            line = e.readline()
            while line:
                f.write(line)
                line = e.readline()
    print('Il y a {} questions '.format(compteur))

    with open(os.path.join(path, 'reponse.tex'), 'w', encoding='latin1') as f:
        with open(current_app.config['LATEX_HOME'] + 'enteteRep.tex', 'r', encoding='utf8') as e:
            line = e.readline()
            while line:
                f.write(line)
                line = e.readline()

        for i in range(1, n + 1):
            numCopie = str(i).rjust(2, '0')
            if i > 1:
                f.write('\\newpage\n')

            f.write('\\begin{minipage}{0.15\\linewidth}\n')
            f.write('\\includegraphics[scale=0.75]{qr3.png}\n')
            f.write('\\end{minipage}\\hfill\n')
            f.write('\\begin{minipage}{0.8\\linewidth}\n')
            f.write('\\dotfill Mathématiques\\dotfill Evaluation, durée : $1$h   \\dotfill 2019-2020.\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\hrule\n')
            f.write('\\vskip0.2cm\n')
            f.write(
                'Pour chaque question, noircir avec un stylo bille noir ou bleu la case correspondant à la bonne réponse.\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\hrule\n')
            f.write('\\vskip0.2cm\n')

            f.write('NUMERO COPIE : ' + numCopie + ' \\hspace{\\stretch{2}} Nom : ' + eleves[i - 1][
                0] + '\\hspace{\\stretch{1}} Prénom : ' + eleves[i - 1][1] + '\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\hrule\n')
            f.write('\\vskip0.2cm\n')
            f.write('\\end{minipage}\n')
            f.write('\\vskip0.5cm\n')
            f.write('\\begin{tabularx}{\linewidth}{@{}lXXXXX@{}}\n')
            f.write('Num & a & b & c & d & e \\\\\n')
            for j in range(1, 20 + 1):
                f.write(str(j).rjust(2,
                                     '0') + '  &  \\fbox{\\strut} & \\fbox{\\strut}  & \\fbox{\\strut}  & \\fbox{\\strut}  & \\fbox{\\strut} \\\\\n')
                f.write('\\\\\n')
            f.write('\\end{tabularx}\n')
        f.write('\\end{document}\n')

    with open(os.path.join(path, 'bonnesReponses.txt'), 'w', encoding='latin1') as f:
        for i in range(1, n + 1):
            numCopie = str(i).rjust(2, '0')
            f.write(numCopie + ';')
            for e in lRep[i - 1]:
                f.write(e + ';')
            f.write('\n')

    return lRep
