#!/bin/bash

eu_countries="andorra austria belarus belgium bulgaria croatia czech+republic denmark estonia finland france germany greece hungary hungary ireland italy latvia lithuania montenegro netherlands norway poland portugal romania serbia slovakia slovenia spain sweden switzerland turkey ukraine united+kingdom"

get_countries() {
    for country in ${eu_countries}; do
        sleep 2
        curl --silent "https://www.ktm.com/en/api/dealer/search?location=${country}&isCountrySearch=true" 2>&1 | iconv -f utf-8 -t ascii//TRANSLIT | sed 's|<p>||g;s|<br/>||g;s|</p>||g;s|,"[a-z]*":null||g' > "tmp/${country}.json"
    done
}

create_csv() {
    rm -f tmp/*.csv
    for i in tmp/*.json; do
        output=`echo $i | sed 's|\.json$|\.csv|'`
        ./ktm_poi -i "${i}" | sed 's|  | |g;s|http://||g' > "${output}"
    done
}

concatenate_all() {
    cat tmp/*.csv >> tmp/ktm_eu.csv
}

mkdir -p tmp
get_countries
create_csv
concatenate_all

