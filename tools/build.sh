#!/usr/bin/env bash
set -ex
trap 'exit' ERR
cd "$(dirname "$0")/.."

#
# Default Global Variables
#
CONFIGURATIONS="debug release"

#
# Helper Functions
#

config_app(){
    PROJ_NAME='app'
    PROJ_DIR='app/pca10056/blank/ses'
}

config_select(){
    local name="${1?}"

    case "${name}" in
        app)
            config_app
            ;;
        *)
            exit 1
    esac
}

build(){
    local BUILD_CONFIG="${1?}"
    local OUTPUT_HEX="${ARTIFACT_DIR}/${config}-${BUILD_CONFIG}-${REVISION}_$(date "+%Y-%m-%d_%H-%M-%S").hex"
    local SIMPLE_HEX="${ARTIFACT_DIR}/${config}-${BUILD_CONFIG}.hex"

    echo "Building ${OUTPUT_HEX}"

    /ses/bin/emBuild -rebuild -config "${BUILD_CONFIG}" -project "${PROJ_NAME}" "${PROJ_DIR}/${PROJ_NAME}.emProject"
    cp "${PROJ_DIR}/Output/${BUILD_CONFIG}/Exe/${PROJ_NAME}.hex" "${OUTPUT_HEX}"
    ln -f ${OUTPUT_HEX} ${SIMPLE_HEX}
}

main(){
    local config="${1-app}"
    config_select "${config?}"

    REVISION="${CI_COMMIT_SHORT_SHA:-$(git describe --match=NeVeRmAtCh --always --abbrev=6 --dirty)}"
    ARTIFACT_DIR='artifacts'

    mkdir -p "${ARTIFACT_DIR}"

    for i in ${CONFIGURATIONS}; do
        build $i
    done
}

main "$@"
