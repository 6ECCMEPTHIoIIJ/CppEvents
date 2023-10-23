#include <iostream>

#include "Observer.h"
#include "ObservableConnector.h"
#include "ObservableInvoker.h"

int main() {
  Observer o;
  o.Bind([](){std::cout << "Hello, World!" << std::endl;});
  ObservableConnector c;
  o.Subscribe(&c);
  ObservableInvoker i(&c);
  i.Invoke();
  o.Unsubscribe();
  i.Invoke();
  o.Subscribe(&c);
  i.Invoke();
  o.Unsubscribe();

  return 0;
}
