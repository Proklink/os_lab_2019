
#!/bin/bash
line=""
commit=""
Line=""
count=0
i=0

while [ -n "$1" ]
do
    let count++
    line="$line $1"
    shift
done

for word in $line
do
    let i++
    if [ "$i" = "$count" ];
    then
        commit="$word"
    else
        Line="$word"
    fi
done

git add $Line
git commit -m "$commit"
git pull
git push