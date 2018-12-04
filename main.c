#include "printf.h"
#include "filesys.h"
#include "timer.h"
#include "storage.h"
#include "malloc.h"

void main() {
    filesys_init();

    // create file 1
    char* success = file_create("newfile.txt", "Bob Marley");
    char* read = file_read("newfile.txt");
    printf("Success Message: %s\n", success);
    printf("Read Normal: %s\n", read);

    // edit file 1
    success = file_update("newfile.txt", "Alex Langshur");
    read = file_read("newfile.txt");
    printf("Success Message: %s\n", success);
    printf("Read Normal: %s\n", read);

    // edit file 1
    success = file_update("newfile.txt", "Alex Day");
    read = file_read("newfile.txt");
    printf("Success Message: %s\n", success);
    printf("Read Normal: %s\n", read);

    // create file 2
    success = file_create("another.txt", "10101001010010101001001010100101");
    read = file_read("another.txt");
    printf("Success Message: %s\n", success);
    printf("Read Normal: %s\n", read);

    // remove file 1
    success = file_delete("newfile.txt");
    read = file_read("newfile.txt");
    printf("Success Message: %s\n", success);
    printf("Read Normal: %s\n", read);

    // create file 2
    success = file_update("another.txt", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris nec velit malesuada urna gravida rhoncus. Etiam sit amet ligula vulputate, blandit orci sed, blandit nulla. Morbi ut diam quam. Ut dictum ipsum porttitor erat ullamcorper, eleifend consectetur elit porta. Ut ante tellus, cursus quis justo a, laoreet porttitor nisi. Nullam eget lectus mi. Mauris efficitur elit at ultricies venenatis. Vestibulum lacus elit, fermentum a commodo sit amet, sagittis ut quam. Aenean pulvinar blandit nulla, nec imperdiet nisi vulputate et. Quisque fringilla nisl elit, eu varius augue pellentesque eu. Donec semper et velit at porta. Aenean non lorem vel dolor bibendum iaculis. Curabitur non turpis id arcu finibus interdum sit amet nec dolor. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Curabitur eu mollis enim. In hac habitasse platea dictumst. Ut non neque cursus, ullamcorper dui nec, rutrum sem. Suspendisse non odio in velit cursus eleifend. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vivamus id volutpat ipsum. Etiam dignissim orci ac lectus congue tristique. In aliquet urna quis tortor finibus, quis porta ipsum eleifend. Morbi sit amet erat id mauris laoreet aliquam eget ut arcu. Nam vel mi rutrum, vestibulum ligula at, tincidunt lectus. Aliquam non ipsum ac lacus accumsan accumsan at non odio. Maecenas ac lacus et tellus mollis finibus. Maecenas vitae diam risus. Phasellus rhoncus erat sit amet faucibus scelerisque. Suspendisse sit amet hendrerit erat. Pellentesque lacinia velit leo, quis rutrum sapien congue ac. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nulla facilisi. Nulla cursus dui justo, eu dictum turpis porttitor sed. Nulla accumsan suscipit pretium. Nulla placerat fringilla lectus et molestie. Mauris dui augue, hendrerit vel leo sit amet, molestie facilisis diam. Nunc dapibus dignissim neque, ac viverra magna ultricies vel. Aliquam tristique cursus quam, a blandit lectus aliquam ut. Proin ut orci tellus. Sed pharetra elit quis lacus sollicitudin viverra. Fusce urna enim, pharetra ac viverra vel, ultrices sit amet dolor. Sed orci odio, convallis ut tortor eu, ullamcorper pretium justo. Quisque tincidunt odio in lectus blandit scelerisque. Aenean ut odio placerat, cursus tortor vehicula, pharetra nulla.");
    read = file_read("another.txt");
    printf("Success Message: %s\n", success);
    while(*read) printf("%c", *read++);
}
