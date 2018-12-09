#include "wire.h"
#include "timer.h"

void main() {
    wire_init(2400, "server");
    wire_write_file("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce porta mauris ac nibh rutrum, sed imperdiet ligula mollis. Vivamus vitae purus sollicitudin, fringilla tellus nec, ultricies orci. Nunc sodales luctus malesuada. Sed magna ipsum, dapibus et gravida a, aliquet eget orci. Donec pretium magna felis, sed ullamcorper felis pulvinar non. Aliquam vitae purus eu diam maximus commodo in et lorem. Suspendisse et consectetur tellus. In finibus leo a bibendum bibendum. Nullam bibendum risus ac ligula sodales rhoncus. Sed suscipit ex quis est mattis, sit amet iaculis libero condimentum. Quisque iaculis quis leo vitae molestie. Duis convallis sodales justo. Aenean in massa ultricies, malesuada sem non, viverra metus. Pellentesque tincidunt volutpat lacus, et varius erat. Vivamus feugiat nec tortor non tincidunt.");
}
