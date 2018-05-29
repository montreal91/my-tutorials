
#include "vertex.h"

const float Vertex::RADIUS = 0.25f;
const float Vertex::SCALE = 30.0f;

Vertex::Vertex(b2World& world) :
_dot(RADIUS * SCALE)
{
  _dot.setFillColor(sf::Color::Cyan);
  _CreatePhysicalBody(world, 0.0f, 0.0f);
}

Vertex::Vertex(b2World& world, float x, float y) :
_dot(RADIUS * SCALE)
{
  _dot.setFillColor(sf::Color::Cyan);
  _CreatePhysicalBody(world, x, y);
}

void Vertex::AddForce(const b2Vec2& force) {
  _body->ApplyForceToCenter(force, true);
}

void Vertex::draw(sf::RenderTarget& target, sf::RenderStates states) const{
  target.draw(_dot, states);
}

b2Vec2 Vertex::GetPosition() const {
  return _body->GetPosition();
}

void Vertex::SetPosition(const b2Vec2& pos) {
  _body->SetTransform(pos, 0);
}

void Vertex::Update() {
  b2Vec2 pos = _body->GetPosition();
  _dot.setPosition(SCALE * pos.x, SCALE * pos.y);
}

void Vertex::_CreatePhysicalBody(b2World& world, float x, float y) {
  b2BodyDef body_def;
  body_def.position = b2Vec2(x, y);
  body_def.type = b2_dynamicBody;

  b2CircleShape circle_shape;
  circle_shape.m_p.Set(0, 0);
  circle_shape.m_radius = RADIUS;

  b2FixtureDef fixture_def;
  fixture_def.density  = 1.0f;
  fixture_def.friction = 0.7f;
  fixture_def.shape = &circle_shape;

  _body = world.CreateBody(&body_def);
  _body->CreateFixture(&fixture_def);

  std::cout << "Mass: " << _body->GetMass() << "\n";
}

float GetDistanceBetweenVertices(const Vertex& v1, const Vertex& v2) {
  b2Vec2 pos1 = v1.GetPosition();
  b2Vec2 pos2 = v2.GetPosition();
  
  b2Vec2 diff = pos2 - pos1;
  return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}
