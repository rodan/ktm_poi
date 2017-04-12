#!/bin/bash

countries="united+kingdom germany austria france greece serbia croatia bosnia bulgaria romania hungary slovenia slovakia czechia netherlands belgium italy spain portugal denmark sweden norway finland monaco switzerland slovakia hungary"

get_countries() {
    for country in ${countries}; do
        sleep 2
        curl --silent "http://www.ktm.com/ro/api/dealer/search?location=${country}&isCountrySearch=true" 2>&1 | iconv -f utf-8 -t ascii//TRANSLIT | sed 's|<p>||g;s|<br/>||g;s|</p>||g;s|,"[a-z]*":null||g' > "tmp/${country}.json"
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
    cat tmp/*.csv >> tmp/europe.csv
}

mkdir -p tmp
get_countries
create_csv
concatenate_all

