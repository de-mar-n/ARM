#!/bin/sh
if [ "$#" != "3" ]; then
  echo "Args: publickey.pem, signature, fichier_sha256"
  exit 1
fi
openssl dgst -sha256 -verify $1 -signature $2 $3

