#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"
SETUP_SCRIPT=$DIR"/fixtures/setup-data.sh" 
UNIT_TESTS=$DIR"/unit"

# prepare data
$SETUP_SCRIPT clear silent
$SETUP_SCRIPT insert

# run tests
phpunit $UNIT_TESTS

# cleanup again
$SETUP_SCRIPT clear
