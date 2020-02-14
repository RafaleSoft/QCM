from flask import (
    Blueprint, flash, g, redirect, render_template, request, url_for
)

from qcm.auth import login_required
from qcm.db import get_db
import csv
import logging

bp = Blueprint('profile', __name__)

niveaux = ['Terminale', 'Première', 'Seconde', 'Troisième', 'Quatrième', 'Cinquième', 'Sixième']


@bp.route('/profile')
@login_required
def index():
    db = get_db()
    teacher = db.execute(
        'SELECT * FROM users'
        ' WHERE id = ?', (g.user['id'],)
    ).fetchone()
    cls = db.execute('SELECT * FROM classes WHERE teacher_id = ?', (g.user['id'],)).fetchall()
    classes = []
    for c in cls:
        classe = {}
        classe['id'] = c['id']
        classe['classname'] = c['classname']
        classe['level'] = niveaux[c['level']]
        els = db.execute('SELECT * FROM students WHERE class_id = ?', (c['id'],)).fetchall()
        classe['eleves'] = els
        classes.append(classe)
    return render_template('profile/teacher.html', teacher=teacher, classes=classes)


@bp.route('/<int:teacher_id>/update', methods=('GET', 'POST'))
@login_required
def update(teacher_id):
    if request.method == 'POST':
        firstname = request.form['firstname']
        lastname = request.form['lastname']
        error = None

        if not firstname:
            error = 'Firstname is required.'
        if not lastname:
            error = 'Lastname is required.'

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'UPDATE users SET firstname = ?, lastname = ?'
                ' WHERE id = ?',
                (firstname, lastname, teacher_id)
            )
            db.commit()
            return redirect(url_for('profile.index'))

    db = get_db()
    teacher = db.execute(
        'SELECT * FROM users'
        ' WHERE id = ?', (g.user['id'],)).fetchone()
    return render_template('profile/teacher_update.html', teacher=teacher)


@bp.route('/<int:teacher_id>/addClassroom', methods=('GET', 'POST'))
@login_required
def addClassroom(teacher_id):
    if request.method == 'POST':
        classname = request.form['classname']
        level = request.form['level']
        error = None

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'INSERT INTO classes (teacher_id, classname, level)'
                ' VALUES (?, ?, ?)',
                (teacher_id, classname, level))
            db.commit()
            return redirect(url_for('profile.index'))

    db = get_db()
    teacher = db.execute(
        'SELECT * FROM users'
        ' WHERE id = ?', (g.user['id'],)).fetchone()
    return render_template('profile/classroom_add.html', teacher=teacher)


@bp.route('/<int:class_id>/classroomUpdate', methods=('GET', 'POST'))
@login_required
def classroomUpdate(class_id):
    if request.method == 'POST':
        firstname = request.form['firstname']
        lastname = request.form['lastname']
        error = None

        if not firstname:
            error = 'Firstname is required.'
        if not lastname:
            error = 'Lastname is required.'

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'INSERT INTO students (class_id, firstname, lastname)'
                ' VALUES (?, ?, ?)',
                (class_id, firstname, lastname))
            db.commit()
            return redirect(url_for('profile.index'))

    db = get_db()
    classe = db.execute('SELECT * FROM classes WHERE id = ?', (class_id,)).fetchone()
    student = {}
    student['firstname'] = 'prenom'
    student['lastname'] = 'nom'
    return render_template('profile/classroom_update.html', classe=classe, eleve=student,
                           title="Ajouter élève à la classe")


@bp.route('/<int:student_id>/editStudent', methods=('GET', 'POST'))
@login_required
def editStudent(student_id):
    if request.method == 'POST':
        firstname = request.form['firstname']
        lastname = request.form['lastname']
        error = None

        if not firstname:
            error = 'Firstname is required.'
        if not lastname:
            error = 'Lastname is required.'

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'UPDATE students SET firstname = ?, lastname = ?'
                ' WHERE id = ?', (firstname, lastname, student_id)
            )
            db.commit()
            return redirect(url_for('profile.index'))

    db = get_db()
    student = db.execute('SELECT * FROM students WHERE id = ?', (student_id,)).fetchone()
    classe = db.execute('SELECT * FROM classes WHERE id = ?', (student['class_id'],)).fetchone()
    return render_template('profile/classroom_update.html', eleve=student, classe=classe, title="Mise à jour élève")


@bp.route('/<int:student_id>/deleteStudent', methods=('GET', 'POST'))
@login_required
def deleteStudent(student_id):
    if request.method == 'POST':
        db = get_db()
        db.execute('DELETE FROM students WHERE id = ?', (student_id,))
        db.commit()
        return redirect(url_for('profile.index'))

    topic = {}
    topic['title'] = 'Retirer l\'élève:'
    db = get_db()
    eleve = db.execute(
        'SELECT * FROM students'
        ' WHERE id = ?', (student_id,)
    ).fetchone()
    topic['object'] = eleve['firstname'] + ' ' + eleve['lastname']
    topic['id'] = student_id
    return render_template('profile/delete.html', topic=topic)


@bp.route('/<int:class_id>/deleteClassroom', methods=('GET', 'POST'))
@login_required
def deleteClassroom(class_id):
    return redirect(url_for('profile.index'))


'''
This structure pass parameters as URL subpath, but application data
is then relative to this path, which is problematic in templates.

@bp.route('/<int:class_id>/loadClassroom', methods=('GET', 'POST'))
'''


@bp.route('/loadClassroom', methods=('GET', 'POST'))
@login_required
def loadClassroom():
    if request.method == 'POST':
        file = request.form['file']
        error = None

        if not file:
            error = 'Filename is required.'
        elif not file.endswith('.csv'):
            error = 'A CSV file is required.'

        if error is not None:
            flash(error)
        else:
            db = get_db()
            class_id = request.args['class_id']

            with open(file) as csvDataFile:
                csv_reader = csv.reader(csvDataFile)
                for row in csv_reader:
                    if len(row) >= 2:
                        firstname = row[0]
                        lastname = row[1]
                        db.execute('INSERT INTO students (class_id, firstname, lastname)'
                                   'VALUES (?, ?, ?)',
                                   (class_id, firstname, lastname))
                    else:
                        logging.warning("Invalid line found in csv input file")

            db.commit()

        return redirect(url_for('profile.index'))

    return render_template('profile/opendatafile.html')
