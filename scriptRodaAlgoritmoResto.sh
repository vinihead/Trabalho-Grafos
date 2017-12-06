max=10

echo "Instancia 30"
for (( i=1; i < $max; ++i ))
do
    ./TrabalhoGrafos instancias/30
done


echo "Instancia 28"
for (( i=5; i < $max; ++i ))
do
    ./TrabalhoGrafos instancias/28
done


echo "Instancia 24"
for (( i=0; i < $max; ++i ))
do
    ./TrabalhoGrafos instancias/24
done
