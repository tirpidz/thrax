set -euo pipefail
# http://redsymbol.net/articles/unofficial-bash-strict-mode/
# -e
# The set -e option instructs bash to immediately exit if any command [1] has a non-zero exit status. 
# -u
# Treat unset variables and parameters other than the special parameters "@" and "*" as an error
# when performing parameter expansion. If expansion is attempted on an unset variable or parameter,
# the shell prints an error message, and, if not interactive, exits with a non-zero status.
# set -o pipefail
# This setting prevents errors in a pipeline from being masked. If any command in a pipeline fails,
# that return code will be used as the return code of the whole pipeline.

REPOSITORY_DIRECTORY="${HOME}/src/thrax"

fetch_thrax() {
    VERSION_NUMBER=$1
    REVISION_NUMBER=$2

    echo "fetching Thrax v${VERSION_NUMBER} r${REVISION_NUMBER}"

    cd ${REPOSITORY_DIRECTORY}
    rm -rf ${REPOSITORY_DIRECTORY}/Thrax*
    wget "http://www.opengrm.org/twiki/bin/viewfile/GRM/ThraxDownload?filename=thrax-${VERSION_NUMBER}.tar.gz;rev=${REVISION_NUMBER}" -O Thrax-v${VERSION_NUMBER}-r${REVISION_NUMBER}.tar.gz
    mkdir Thrax
    tar -xf *gz -C Thrax --strip-components 1
    git add .
    git commit -am "feat(Thrax): ${VERSION_NUMBER} rev ${REVISION_NUMBER}"
    git tag -am v${VERSION_NUMBER}-r${REVISION_NUMBER} "thrax-v${VERSION_NUMBER}-r${REVISION_NUMBER}"
    git push --tags
    sleep 5
    echo "fetching done"
    echo "-------------"
}

fetch_thrax 0.1.0 1

fetch_thrax 1.0.0 1
fetch_thrax 1.0.0 2
fetch_thrax 1.0.1 1
fetch_thrax 1.0.1 2
fetch_thrax 1.0.2 1

fetch_thrax 1.1.0 1
fetch_thrax 1.1.0 2
fetch_thrax 1.1.0 3
fetch_thrax 1.1.0 4

fetch_thrax 1.2.0 1
fetch_thrax 1.2.1 1
fetch_thrax 1.2.1 2
fetch_thrax 1.2.2 1
fetch_thrax 1.2.3 1
fetch_thrax 1.2.3 2
fetch_thrax 1.2.4 1
fetch_thrax 1.2.4 2
fetch_thrax 1.2.5 1
fetch_thrax 1.2.6 1
fetch_thrax 1.2.7 1
fetch_thrax 1.2.8 1
fetch_thrax 1.2.9 1

fetch_thrax 1.3.0 1
fetch_thrax 1.3.1 1
fetch_thrax 1.3.2 1
fetch_thrax 1.3.3 1
fetch_thrax 1.3.4 1
fetch_thrax 1.3.5 1
fetch_thrax 1.3.6 1
