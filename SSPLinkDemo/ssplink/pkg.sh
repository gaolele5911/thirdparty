
echo "[SVP] PKG: Path    : "`pwd`

if [ "$ARCH" != "arm" ]; then
    echo "[SVP] PKG: ARCH    : no arch, use \"arm\" by default."
    ARCH=arm #debug
else
    echo "[SVP] PKG: ARCH    : $ARCH"
fi

dist=../../../../dist/release/sva/app-pkg

mkdir -p ../../../../dist/release/sva/app-pkg

get_pkg_name()
{
    grep "<package>" ./manifest.xml | sed 's/.*<.*>\([^<].*\)<.*>.*/\1/'
}

get_pkg_version()
{
     grep "<version>" ./manifest.xml | sed 's/.*<.*>\([^<].*\)<.*>.*/\1/'
}

pkg_name=`get_pkg_name`
pkg_version=`get_pkg_version`

echo "[SVP] PKG: Name    : $pkg_name"
echo "[SVP] PKG: Version : $pkg_version"

pkg_dist_name=$pkg_name-$pkg_version.pkg
pkg_dist_path=$dist"/"$pkg_dist_name

echo "[SVP] PKG: Output  : $pkg_dist_path"

pkg_content=""

if [  ! -f manifest.xml ]; then
    echo "[SVP] PKG: Fail, no manifest.xml!"
    exit 0
fi
pkg_content="$pkg_content manifest.xml"

if [  ! -s bin ]; then
    echo "[SVP] PKG: Fail, no bin or bin is empty!"
    exit 0
fi
pkg_content="$pkg_content bin"

if [ -s lib ]; then
    pkg_content="$pkg_content lib"
fi

if [ -s res ]; then
    pkg_content="$pkg_content res"
fi

tar -czvf $pkg_dist_path --exclude=*_d --exclude=*.a $pkg_content > $pkg_dist_path.log


