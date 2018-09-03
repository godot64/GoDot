#!/usr/bin/env bash

DIR="$(dirname $0)"
source "${DIR}/acme-version.sh"

rm -rf "${WORK_DIR}"
mkdir -p "${WORK_DIR}"
wget https://github.com/jan0sch/acme-crossassembler/archive/${ACME_VERSION}.tar.gz -O ${WORK_DIR}/acme.tar.gz
tar zxf tmp/acme.tar.gz -C ${WORK_DIR}

make -C ${ACME_DIR}
st=$?

if [ "$st" -eq 0 ]; then
  echo "Created ACME executable in ${ACME_DIR}/."
  exit 0
else
  echo "Could not compile ACME crossassembler!"
  exit 1
fi

