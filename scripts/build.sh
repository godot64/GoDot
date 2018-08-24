#!/usr/bin/env bash

DIR="$(dirname $0)"
source "${DIR}/acme-version.sh"

export PATH="${ACME_DIR}:${PATH}"
make d81

