#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>


int main() {
  using namespace ftxui;

  auto document = vbox({
    text("Merhaba!"),
    text("FTXUI ile terminal arayüzü."),
  }) | border;

  auto screen = Screen::Create(Dimension::Fit(document));
  Render(screen, document);
  screen.Print();

  return 0;
}