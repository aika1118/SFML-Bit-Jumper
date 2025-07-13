#include "MyDebugDraw.h"

MyDebugDraw::MyDebugDraw(RenderTarget& target)
	: _target(target)
{

}

void MyDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	ConvexShape shape(vertexCount);
	for (int i = 0; i < vertexCount; ++i) // convex에 vertices 좌표 set
		shape.setPoint(i, Vector2f(vertices[i].x, vertices[i].y));


	shape.setFillColor(Color::Transparent);
	shape.setOutlineThickness(0.02f);
	shape.setOutlineColor(Color((Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 255)));
	_target.draw(shape);
}

void MyDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	ConvexShape shape(vertexCount);
	for (int i = 0; i < vertexCount; ++i) // convex에 vertices 좌표 set
		shape.setPoint(i, Vector2f(vertices[i].x, vertices[i].y));


	shape.setFillColor(Color((Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 120)));
	_target.draw(shape);
}

void MyDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	CircleShape circle(radius);
	circle.setPosition(center.x, center.y);
	circle.setOrigin(radius, radius);
	circle.setFillColor(Color::Transparent);
	circle.setOutlineThickness(0.02f);
	circle.setOutlineColor(Color((Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 255)));
	_target.draw(circle);
}

void MyDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	CircleShape circle(radius);
	circle.setPosition(center.x, center.y);
	circle.setOrigin(radius, radius);
	circle.setFillColor(Color((Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 120)));
	_target.draw(circle);

	b2Vec2 p = center + (radius * axis); // 원의 중심에서 축을 기준으로 한 점까지의 선을 그리기
	DrawSegment(center, p, color); // center에서 p까지의 선분을 주어진 색상으로 그림
}

void MyDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	VertexArray va(Lines, 2);
	Color sfColor = Color((Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 120));
	va[0].position = Vector2f(p1.x, p1.y);
	va[0].color = sfColor;

	va[1].position = Vector2f(p2.x, p2.y);
	va[1].color = sfColor;

	_target.draw(va);
}

// 오브젝트 중심축에서 x, y축 방향 그리기
void MyDebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p = xf.p;
	b2Vec2 px = p + (0.5f * xf.q.GetXAxis());
	b2Vec2 py = p + (0.5f * xf.q.GetYAxis());

	DrawSegment(p, px, b2Color(1.f, 0.f, 0.f));
	DrawSegment(p, py, b2Color(0.f, 1.f, 0.f));
}

void MyDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	CircleShape circle(size);
	circle.setPosition(p.x, p.y);
	circle.setOrigin(size, size);
	circle.setFillColor(Color((Uint8)(color.r * 255), (Uint8)(color.g * 255), (Uint8)(color.b * 255), (Uint8)(color.a * 120)));
	_target.draw(circle);
}

