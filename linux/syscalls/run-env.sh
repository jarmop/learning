nasm -felf64 env.asm -o env.o
# nasm -f bin env.asm -o a.out
ld -o a.out env.o
chmod +x a.out
./a.out