from flask import (
    Blueprint, flash, g, redirect, render_template, request, url_for
)
from werkzeug.exceptions import abort

from qcm.auth import login_required
from qcm.db import get_db

bp = Blueprint('blog', __name__)

niveaux = ['Terminale', 'Première', 'Seconde', 'Troisième', 'Quatrième', 'Cinquième', 'Sixième']


@bp.route('/')
def index():
    db = get_db()
    cls = db.execute(
        'SELECT * FROM classes'
        ' WHERE teacher_id = ?', (g.user['id'],)
    ).fetchall()
    classes = []
    for c in cls:
        classe = {}
        classe['id'] = c['id']
        classe['classname'] = c['classname']
        classe['level'] = niveaux[c['level']]
        els = db.execute(
            'SELECT * FROM students'
            ' WHERE class_id = ?', (c['id'],)
        ).fetchall()
        print('eleves:', len(els))
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
