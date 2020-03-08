import os

from flask import (
    Blueprint, flash, g, redirect,
    render_template, request, url_for,
    current_app)

from qcm.auth import login_required
from qcm.db import get_db
import csv

bp = Blueprint('profile', __name__)

niveaux = ['Terminale', 'Première', 'Seconde', 'Troisième', 'Quatrième', 'Cinquième', 'Sixième']
modal_form = {'display': 'none', 'text': 'Empty text', 'class_id': 0}


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
        classe = {'id': c['id'], 'classname': c['classname'], 'level': niveaux[c['level']]}
        els = db.execute('SELECT * FROM students WHERE class_id = ?', (c['id'],)).fetchall()
        classe['eleves'] = els
        classes.append(classe)
    return render_template('profile/teacher.html', teacher=teacher, classes=classes, modal=modal_form)


@bp.route('/<int:teacher_id>/update', methods=('GET', 'POST'))
@login_required
def update(teacher_id):
    if request.method == 'POST':
        firstname = request.form['firstname']
        lastname = request.form['lastname']
        email = request.form['email']
        sendevalemail = request.form['sendevalemail']
        sendrecallemail = request.form['sendrecallemail']
        error = None

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'UPDATE users SET firstname = ?, lastname = ?, email = ?, sendevalemail = ?, sendrecallemail = ?'
                ' WHERE id = ?',
                (firstname, lastname, email, sendevalemail, sendrecallemail, teacher_id)
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

    topic = {'title': 'Retirer l\'élève:'}
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
    global modal_form

    if 'none' == modal_form['display']:
        db = get_db()
        classe = db.execute('SELECT * FROM classes WHERE id = ?', (class_id,)).fetchone()
        modal_form['display'] = 'block'
        modal_form['text'] = 'Supprimer la classe ' + classe['classname'] + ' ?'
        modal_form['class_id'] = class_id
    else:
        modal_form['display'] = 'none'
        modal_form['text'] = 'Texte vide'
        modal_form['class_id'] = 0

        if request.args.__contains__('action'):
            action = request.args['action']
            if action == 'yes':
                db = get_db()
                db.execute('DELETE from students WHERE class_id = ?', (class_id,))
                db.execute('DELETE from classes WHERE id = ?', (class_id,))
                db.execute('DELETE from evaluations WHERE class_id = ?', (class_id,))
                db.commit()
                current_app.logger.info("Classe %d supprimée.", class_id)
            elif action == 'no':
                current_app.logger.info("Classe %d non supprimée.",class_id)
            else:
                flash("Erreur interne")

    return redirect(url_for('profile.index'))


@bp.route('/loadClassroom', methods=('GET', 'POST'))
@login_required
def loadClassroom():
    if request.method == 'POST':
        file_storage = request.files['file']
        error = None

        if not file_storage:
            error = 'Filename is required.'
        elif not file_storage.filename.endswith('.csv'):
            error = 'A CSV file is required.'

        if error is not None:
            flash(error)
        else:
            class_id = request.args['class_id']
            base_path = os.path.join(current_app.instance_path, 'users', str(class_id))
            file = os.path.join(base_path, file_storage.filename)
            file_storage.save(file)

            db = get_db()

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
                        current_app.logger.warning("Invalid line found in csv input file")

            db.commit()

        return redirect(url_for('profile.index'))

    return render_template('profile/opendatafile.html')
