tcc lex.c compiler.c parse.c -o cosc
./cosc casm/test.c > a
cat a
echo "-----"
casm a a > b
casm r b

rm -rf a
rm -rf b
rm -rf cosc
