from flask import (
    Blueprint, flash, g, redirect, render_template, request, url_for
)
from werkzeug.exceptions import abort

from qcm.auth import login_required
from qcm.db import get_db

bp = Blueprint('profile', __name__)


@bp.route('/profile')
@login_required
def index():
    db = get_db()
    teacher = db.execute(
        'SELECT * FROM users'
        ' WHERE id = ?', (g.user['id'],)
    ).fetchone()
    return render_template('profile/teacher.html', teacher=teacher)


@bp.route('/<int:teacher_id>/update', methods=('GET', 'POST'))
@login_required
def update(teacher_id):
    if request.method == 'POST':
        firstname = request.form['firstname']
        lastname = request.form['lastname']
        error = None

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
        ' WHERE id = ?', (g.user['id'],)
    ).fetchone()
    return render_template('profile/teacher_update.html', teacher=teacher)
