BASEDIR=`dirname $0`
CURRENT_PATH=`cd $BASEDIR; pwd`

export QML_IMPORT_PATH=$CURRENT_PATH/External/QmlMaterial
export QML2_IMPORT_PATH=$QML_IMPORT_PATH
/home/user/devel/qtcreator-4.2.0/bin/qtcreator
