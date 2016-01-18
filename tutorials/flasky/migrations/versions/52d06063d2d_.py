"""empty message

Revision ID: 52d06063d2d
Revises: 5369e4fb686
Create Date: 2016-01-17 20:24:48.197718

"""

# revision identifiers, used by Alembic.
revision = '52d06063d2d'
down_revision = '5369e4fb686'

from alembic import op
import sqlalchemy as sa


def upgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.add_column('users', sa.Column('confirmed', sa.Boolean(), nullable=True))
    ### end Alembic commands ###


def downgrade():
    ### commands auto generated by Alembic - please adjust! ###
    op.drop_column('users', 'confirmed')
    ### end Alembic commands ###
