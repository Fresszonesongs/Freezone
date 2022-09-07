#!/bin/bash

echo /tmp/core | tee /proc/sys/kernel/core_pattern
ulimit -c unlimited

# if we're not using PaaS mode then start freezoned traditionally with sv to control it
if [[ ! "$USE_PAAS" ]]; then
  mkdir -p /etc/service/freezoned
  cp /usr/local/bin/freezone-sv-run.sh /etc/service/freezoned/run
  chmod +x /etc/service/freezoned/run
  runsv /etc/service/freezoned
elif [[ "$IS_TESTNET" ]]; then
  /usr/local/bin/pulltestnetscripts.sh
else
  /usr/local/bin/startpaasfreezoned.sh
fi
