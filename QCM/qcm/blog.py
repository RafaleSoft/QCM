from flask import (
    Blueprint, flash, g, redirect, render_template, request, url_for, current_app, send_file
)

from werkzeug.exceptions import abort

from qcm.auth import login_required
from qcm.db import get_db
from qcm.qcm import txt2qcm, qcm2tex

import os
from pyscan import qrgen

bp = Blueprint('blog', __name__)
niveaux = ['Terminale', 'Première', 'Seconde', 'Troisième', 'Quatrième', 'Cinquième', 'Sixième']

'''
TODO: tester:
https://www.w3schools.com/csSref/tryit.asp?filename=trycss_sel_hover_dropdown
'''


@bp.route('/')
@login_required
def index():
    db = get_db()
    cls = db.execute(
        'SELECT * FROM classes'
        ' WHERE teacher_id = ?', (g.user['id'],)
    ).fetchall()
    classes = []
    for c in cls:
        classe = {'id': c['id'], 'classname': c['classname'], 'level': niveaux[c['level']]}
        els = db.execute(
            'SELECT * FROM students'
            ' WHERE class_id = ?', (c['id'],)
        ).fetchall()
        classe['eleves'] = len(els)

        evals = db.execute(
            'SELECT * FROM evaluations'
            ' WHERE class_id = ?', (c['id'],)
        ).fetchall()
        classe['evaluations'] = evals
        classes.append(classe)

    '''
    posts = db.execute(
        'SELECT p.id, title, body, created, author_id, username'
        ' FROM post p JOIN users u ON p.author_id = u.id'
        ' ORDER BY created DESC'
    ).fetchall()'''

    return render_template('blog/index.html', classes=classes)


@bp.route('/genevaluation', methods=('GET', 'POST'))
@login_required
def genevaluation():
    if request.method == 'POST':
        question_storage = request.files['question']
        date = request.form['date']
        error = None

        if not question_storage:
            error = "Un fichier d'énoncé est requis."
        if not date:
            error = "Une date d'évaluation est requise."

        if error is not None:
            flash(error)
        else:
            class_id = request.args['class_id']
            db = get_db()
            els = db.execute(
                'SELECT * FROM students'
                ' WHERE class_id = ?', (class_id,)
            ).fetchall()

            '''
                Generate a QR code for each student
            '''
            base_path = os.path.join(current_app.instance_path, 'users')
            base_path = os.path.join(base_path, str(class_id))
            question_storage.save(os.path.join(base_path, question_storage.filename))
            eleves = []
            i = 0
            for e in els:
                eleves.append([e['lastname'], e['firstname']])
                qr = os.path.join(base_path, 'qr') + str(i) + '.png'
                qrgen(e['lastname'] + "-" + e['firstname'], qr)
                i = i + 1

            '''
                Generate a pdf files
            '''
            filename = os.path.join(base_path, question_storage.filename)
            questions = txt2qcm(filename)
            if len(questions) > 0:
                qcm2tex(questions, len(els), base_path, eleves)
                base_cmd = current_app.config['LATEX_HOME'] + \
                           current_app.config['LATEX_BIN'] + \
                           ' -output-directory=' + base_path + ' '
                os.system(base_cmd + os.path.join(base_path, 'sujet.tex'))
                os.system(base_cmd + os.path.join(base_path, 'reponse.tex'))

            '''
                Do some cleaning of all intermediate files
            '''
            i = 0
            for e in els:
                qr = os.path.join(base_path, 'qr') + str(i) + '.png'
                os.unlink(qr)
                i = i + 1
            os.unlink(os.path.join(base_path, 'reponse.aux'))
            os.unlink(os.path.join(base_path, 'reponse.log'))
            os.unlink(os.path.join(base_path, 'reponse.tex'))
            os.unlink(os.path.join(base_path, 'sujet.aux'))
            os.unlink(os.path.join(base_path, 'sujet.log'))
            os.unlink(os.path.join(base_path, 'sujet.tex'))

            datepath = os.path.join(base_path, date)
            os.rename(os.path.join(base_path, 'sujet.pdf'), datepath + '_sujet.pdf')
            os.rename(os.path.join(base_path, 'reponse.pdf'), datepath + '_reponse.pdf')

            db.execute(
                'INSERT INTO evaluations (class_id, date_eval,subject)'
                ' VALUES (?, ?, ?)',
                (class_id, date, filename))
            db.commit()

            return redirect(url_for('blog.index'))

    return render_template('blog/opendatafile.html')


@bp.route('/scanevaluation', methods=('GET', 'POST'))
@login_required
def scanevaluation():
    if request.method == 'POST':
        return redirect(url_for('blog.index'))

    return render_template('blog/opendatafile.html')


@bp.route('/downloadevaluation/<int:class_id>/<path>')
@login_required
def downloadevaluation(class_id=0, path=None):
    if path is None:
        abort(404)
    if class_id == 0:
        abort(404)

    fp = os.path.join(current_app.instance_path, 'users', str(class_id), path)

    if os.path.exists(fp):
        return send_file(fp)
    else:
        abort(404)


@bp.route('/create', methods=('GET', 'POST'))
@login_required
def create():
    if request.method == 'POST':
        title = request.form['title']
        body = request.form['body']
        error = None

        if not title:
            error = 'Title is required.'

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'INSERT INTO post (title, body, author_id)'
                ' VALUES (?, ?, ?)',
                (title, body, g.user['id'])
            )
            db.commit()
            return redirect(url_for('blog.index'))

    return render_template('blog/create.html')


def get_post(post_id, check_author=True):
    post = get_db().execute(
        'SELECT p.id, title, body, created, author_id, username'
        ' FROM post p JOIN users u ON p.author_id = u.id'
        ' WHERE p.id = ?',
        (post_id,)
    ).fetchone()

    if post is None:
        abort(404, "Post id {0} doesn't exist.".format(post_id))

    if check_author and post['author_id'] != g.user['id']:
        abort(403)

    return post


'''
@bp.route('/<int:post_id>/update', methods=('GET', 'POST'))
@login_required
def update(post_id):
    post = get_post(post_id)

    if request.method == 'POST':
        title = request.form['title']
        body = request.form['body']
        error = None

        if not title:
            error = 'Title is required.'

        if error is not None:
            flash(error)
        else:
            db = get_db()
            db.execute(
                'UPDATE post SET title = ?, body = ?'
                ' WHERE id = ?',
                (title, body, post_id)
            )
            db.commit()
            return redirect(url_for('blog.index'))

    return render_template('blog/update.html', post=post)
'''


@bp.route('/<int:post_id>/delete', methods=('POST',))
@login_required
def delete(post_id):
    get_post(post_id)
    db = get_db()
    db.execute('DELETE FROM post WHERE id = ?', (post_id,))
    db.commit()
    return redirect(url_for('blog.index'))
