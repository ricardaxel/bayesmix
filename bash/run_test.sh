#!/usr/bin/env bash

# build/run \
#   algo_params_file \
#   hier_type hier_args \
#   mix_type mix_args \
#   collname \
#   datafile \
#   gridfile \
#   densfile \
#   massfile \
#   nclufile \
#   clusfile \
#   [hier_cov_file] \
#   [hier_grid_cov_file] \
#   [mix_cov_file] \
#   [mix_grid_cov_file]

if [ "$1" == 'uni' ]; then
  build/run \
    algo_settings.asciipb \
    NNIG resources/asciipb/nnig_ngg_prior.asciipb \
    DP   resources/asciipb/dp_gamma_prior.asciipb \
    "" \
    resources/csv/in/data_uni.csv \
    resources/csv/in/grid_uni.csv \
    resources/csv/out/uni_dens.csv \
    resources/csv/out/uni_mass.csv \
    resources/csv/out/uni_nclu.csv \
    resources/csv/out/uni_clus.csv
elif [ "$1" == 'multi' ]; then
  build/run \
    algo_settings.asciipb \
    NNW resources/asciipb/nnw_ngiw_prior.asciipb \
    DP  resources/asciipb/dp_gamma_prior.asciipb \
    "" \
    resources/csv/in/data_multi.csv \
    resources/csv/in/grid_multi.csv \
    resources/csv/out/multi_dens.csv \
    resources/csv/out/multi_mass.csv \
    resources/csv/out/multi_nclu.csv \
    resources/csv/out/multi_clus.csv
elif [ "$1" == 'lru' ]; then
  build/run \
    algo_settings.asciipb \
    LinRegUni resources/asciipb/lin_reg_uni_fixed.asciipb \
    DP        resources/asciipb/dp_gamma_prior.asciipb \
    "" \
    resources/csv/in/data_lru.csv \
    resources/csv/in/covs_grid_lru.csv \
    resources/csv/out/lru_dens.csv \
    resources/csv/out/lru_mass.csv \
    resources/csv/out/lru_nclu.csv \
    resources/csv/out/lru_clus.csv \
    resources/csv/in/covs_lru.csv \
    resources/csv/in/covs_grid_lru.csv
else
  echo 'Syntax: bash/run_test.sh followed by one of: uni, multi, lru'
fi
