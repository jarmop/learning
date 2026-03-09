gcc -Wall -Wextra -O2 main.c mini_egl.c mini_backend_custom.c mini_driver_hook_egl.c $(pkg-config --cflags --libs libdrm gbm egl glesv2)

sshpass -f passw scp -P 2223 a.out jarmovm@localhost:

# -t flag makes the running program terminate when the ssh connection closes
#    (More specifically, the -t flag makes ssh open up a pseudo-tty which is closed 
#    when the ssh connection closes and that causes the kernel to emit SIGHUP to 
#    the foreground process group effectively terminating each process)
sshpass -f passw ssh -t -p 2223 jarmovm@localhost ./a.out