
#include "application.h"


const float Application::EPSILON = 1;
const sf::Time Application::TIME_PER_FRAME = sf::seconds(1.0f / 60.0f);

Application::Application() :
adjacency_matrix(),
physical_world(b2Vec2(0.0f, 0.0f)),
vertices(),
window(sf::VideoMode(1200, 600), "Diploma", sf::Style::Close) {
  std::ifstream fileinput("graph.txt");
  this->LoadData(fileinput);
}

void Application::Run() {
  sf::Clock clock;
  sf::Time time_since_last_update = sf::Time::Zero;
  while (window.isOpen()) {
    sf::Time dt = clock.restart();
    time_since_last_update += dt;

    while (time_since_last_update > TIME_PER_FRAME) {
      time_since_last_update -= TIME_PER_FRAME;
      this->ProcessInput();
      this->Update(TIME_PER_FRAME);
      this->Render();
    }
  }
}

b2Vec2 Application::CalculateForceDirection(const Vertex& subject, const Vertex& object) const {
  b2Vec2 res = subject.GetPosition();
  res -= object.GetPosition();
  return util::GetNormalizedVector(res);
}

float Application::CalculateForceMagnitude(const Vertex& subject, const Vertex& object) const{
  return GRAVITATIONAL_CONSTANT;
}

void Application::LoadData(std::istream& in) {
  int n;
  in >> n;
  std::cout << "Number of vertices: " << n << "\n";
  for (int i = 0; i < n; i++) {
    int x, y;
    in >> x >> y;
    Vertex v(this->physical_world, x, y);
    this->vertices.push_back(v);
  }
  float tmp = 0;

  in >> this->GRAVITATIONAL_CONSTANT;
  std::cout << "Gravitaional constant: " << this->GRAVITATIONAL_CONSTANT << "\n";

  this->adjacency_matrix.clear();
  for (int i = 0; i < n; i++) {
    this->adjacency_matrix.push_back(std::vector<float>());
    for (int j = 0; j < n; j++) {
      in >> tmp;
      this->adjacency_matrix[i].push_back(tmp);
    }
  }
}

void Application::PrintTestData() const {
  b2Vec2 position = this->vertices[0].GetPosition();
  std::cout << "Pos: " << position.x << " " << position.y << "\n";
}

void Application::ProcessInput() {
  sf::Event event;
  while (this->window.pollEvent(event)) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Q) {
        this->window.close();
      }
      if (event.key.code == sf::Keyboard::P) {
        this->PrintTestData();
      }
    }
    if (event.type == sf::Event::Closed) {
      std::cout << "ProcessInput close window\n";
      this->window.close();
    }
  }
}

void Application::Render() {
  this->window.clear();
  for (Vertex vertex : this->vertices) {
    this->window.draw(vertex);
  }
  this->window.display();
}

void Application::Update(const sf::Time& dt) {
  for (int i=0; i<this->vertices.size(); i++) {
    for (int j=0; j<this->vertices.size(); j++) {
      b2Vec2 force = this->CalculateForceDirection(this->vertices[i], this->vertices[j]);
      float magnitude = this->CalculateForceMagnitude(this->vertices[i], this->vertices[j]);
      force *= magnitude;
      force *= -this->adjacency_matrix[i][j];
      this->vertices[i].AddForce(force);
    }
  }
  this->physical_world.Step(dt.asSeconds(), 8, 3);
  for (auto i = 0; i < this->vertices.size(); i++) {
    this->vertices[i].Update(dt);
  }
}

b2Vec2 util::GetNormalizedVector(const b2Vec2& vec) {
  float norma = util::GetVectorNorm(vec);
  if (norma <= 0.0001) {
    return b2Vec2(vec.x, vec.y);
  }

  b2Vec2 res = b2Vec2(vec.x / norma, vec.y / norma);
  return res;
}

float util::GetVectorNorm(const b2Vec2& vec) {
  return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}
