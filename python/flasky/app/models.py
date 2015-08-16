
from werkzeug.security  import generate_password_hash
from werkzeug.security  import check_password_hash

from flask.ext.login    import UserMixin

from .                  import db
from .                  import login_manager


class XRole( db.Model ):
    __tablename__   = "roles"
    pk              = db.Column( db.Integer, primary_key= True )
    name            = db.Column( db.String( 64 ), unique=True )
    default         = db.Column( db.Boolean, default=False, index=True )
    permissions     = db.Column( db.Integer )
    users           = db.relationship( "XUser", backref="role" )

    @staticmethod
    def InsertRoles():
        roles = {
            "User": ( 
                XPermission.FOLLOW | 
                XPermission.COMMENT | 
                XPermission.WRITE_ARTICLES, 
                True,
            ),
            "Moderator": (
                XPermission.FOLLOW | 
                XPermission.COMMENT | 
                XPermission.WRITE_ARTICLES |
                XPermission.MODERATE_COMMENTS,
                False,
            ),
            "Administrator": ( 0xff, False ),
        }

        for r in roles:
            role = XRole.query.filter_by( name=r ).first()
            if role is None:
                role = Role( name=r )
            role.permissions = roles[ r ][ 0 ]
            role.default = roles[ r ][ 1 ]
            db.session.add( role )
        db.session.commit()

    def __repr__( self ):
        return "<Role %r>" % self.name


class XUser( UserMixin, db.Model ):
    __tablename__   = "users"
    pk              = db.Column( db.Integer, primary_key=True )
    username        = db.Column( db.String( 64 ), unique=True, index=True )
    email           = db.Column( db.String( 64 ), unique=True, index=True )
    role_pk         = db.Column( db.Integer, db.ForeignKey( "roles.pk" ) )
    password_hash   = db.Column( db.String( 128 ) )


    @property
    def password( self ):
        raise AttributeError( "password is not a readable attribute" )


    @password.setter
    def password( self, password ):
        self.password_hash = generate_password_hash( password )


    def VerifyPassword( self, password ):
        return check_password_hash( self.password_hash, password )


    def get_id( self ):
        return self.pk
    

    def __repr__( self ):
        return "<User %r>" % self.username


@login_manager.user_loader
def load_user( user_id ):
    return XUser.query.get( int( user_id ) )


class XPermission:
    FOLLOW = 0x01
    COMMENT = 0x02
    WRITE_ARTICLES = 0x04
    MODERATE_COMMENTS = 0x08
    ADMINISTER = 0x80

