#! /bin/bash

CUR_DIR=$(cd "$(dirname "$0")";pwd)

$CUR_DIR/build_tool/remake.sh $1

DEBUG_MODE=false
if [ "$1" == "-DEBUG" ]; then
    DEBUG_MODE=true
fi

PLATFORM=$(uname -m)
IS_ARM=false
IS_X86=false
if [ $PLATFORM == 'x86_64' ]; then
    IS_X86=true
fi
if [ $PLATFORM == 'aarch64' ]; then
    IS_ARM=true
fi

if [[ "$IS_X86" == "false" && "$IS_ARM" == "false" ]]; then
    echo -e "NOT support platform $PLATFORM"
    exit -1
fi

DRRUN=$CUR_DIR/../build/bin64/drrun
if [ "$DEBUG_MODE" == "true" ]; then
    DRRUN=$CUR_DIR/../build_debug/bin64/drrun
fi

DEBUG_FLAG=
if [ "$DEBUG_MODE" == "true" ]; then
    DEBUG_FLAG=-debug
fi

ARM_SPECIAL_FLAG=
if [ "$IS_ARM" == "true" ]; then
    ARM_SPECIAL_FLAG=-unsafe_build_ldstex
fi

LOG_PATH=$CUR_DIR/../logs
if [ ! -d $LOG_PATH ]; then
    mkdir $LOG_PATH
fi


INPUT=5
LULESH_FULL_PATH=/home/ubuntu/DrCCTProf-ARM-samples/lulesh/build/lulesh-par-original

cd $LOG_PATH
export OMP_NUM_THREADS=1
# export OMP_DYNAMIC=FALSE
# export KMP_SCHEDULE=static,balanced
export GOMP_CPU_AFFINITY="0-63"
time  $LULESH_FULL_PATH $INPUT

time $DRRUN $DEBUG_FLAG $ARM_SPECIAL_FLAG -t drcctlib_all_instr_cct_no_cache -- $LULESH_FULL_PATH $INPUT

time $DRRUN $DEBUG_FLAG $ARM_SPECIAL_FLAG -t drcctlib_all_instr_cct -- $LULESH_FULL_PATH $INPUT

time $DRRUN $DEBUG_FLAG $ARM_SPECIAL_FLAG -t drcctlib_instr_statistics -- $LULESH_FULL_PATH $INPUT

time $DRRUN $DEBUG_FLAG $ARM_SPECIAL_FLAG -t drcctlib_reuse_distance -- $LULESH_FULL_PATH $INPUT

time $DRRUN $DEBUG_FLAG $ARM_SPECIAL_FLAG -t drcctlib_reuse_distance_client_cache -- $LULESH_FULL_PATH $INPUT

