#pragma once
#ifndef VOR_INCREMENTAL
#define VOR_INCREMENTAL

#include "Algorithm.h"
#include "DCEL.h"
#include <gl/gl.h>
#include <gl/glut.h>
#include <Windows.h>


class VOR_Incremental : public Algorithm
{
private:
	HalfEdge* U;
	HalfEdge* D;
	HalfEdge* R;
	HalfEdge* L;
public:
	VOR_Incremental (){}
	Point normalize(Point p)
	{
		double m = sqrt(p.x*p.x+p.y*p.y);
		p.x /= m;
		p.y /= m;
		return p;
	}
	void DrawHalfEdge(Point p1, Point p2, string name = "")
	{
		float differenceBetweenEdges = 1;
		float distance = 20;
		Point v1,v2;
		v1.x = (p2.x - p1.x);
		v1.y = (p2.y - p1.y);
		v1 = normalize(v1);
		v2.x = (-v1.y);
		v2.y = (v1.x);
		p1.x = (p1.x + (v1.x*differenceBetweenEdges) + (differenceBetweenEdges*v2.x));
		p1.y = (p1.y + (v1.y*differenceBetweenEdges) + (differenceBetweenEdges*v2.y));
		p2.x = (p2.x - (v1.x*differenceBetweenEdges) + (differenceBetweenEdges*v2.x));
		p2.y = (p2.y - (v1.y*differenceBetweenEdges) + (differenceBetweenEdges*v2.y));
		drawLine(Line(p1,p2));
		Point p3;
		p3.x = (p2.x-(distance*v1.x));
		p3.y = (p2.y-(distance*v1.y));
		p3.x = (p3.x+(distance*v2.x));
		p3.y = (p3.y+(distance*v2.y));
		drawLine(Line(p2,p3));
		glFlush();

		//char str[10];
		//Point m((p1.x+p2.x)/2, (p1.y+p2.y)/2);
		//glRasterPos2f(m.x+5, m.y+5);
		//
		//for(unsigned i = 0; i < name.size(); i ++)
		//	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
	void DrawHalfEdge(HalfEdge* h, string n = "")
	{
		DrawHalfEdge(h->getStart(), h->getEnd(), n);
	}
	void DrawEdge(Point p1, Point p2)
	{
		DrawHalfEdge(p1,p2);
		DrawHalfEdge(p2,p1);
	}
	void drawDCEL(const DCEL& d)
	{
	}
	//Return Segment perpendicaular at segment between p1 and p2
	//Cutting the boundries.


	Line getPerpendicular2Points(Point p1, Point p2)
	{
		//TODO
		const float OO = 10000;
		Point MP((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
		float deltaY = p2.y - p1.y;
		float deltaX = p2.x - p1.x;
		if((deltaY - EPS) < 0)
			return Line(Point(MP.x, -OO), Point(MP.x, OO));
		float m = (-1 * deltaX) / deltaY;
		float b = MP.y - m * MP.x;
		float x = 10000.0; // large factor
		Point R1(x, m * x + b), R2(-x, -1 * m * x + b);
		return Line(R1, R2);
	}
	// get two line and return intersection point
	Point getIntersectionPoints(Line Perpend, Line L) {
		//TODO
		return Point();
	}



	//Return index of the face that closest to newPoint from DCEL
	//if tie, return any
	int getNearestFace(Point newPoint, DCEL* dcel)
	{
		//TODO
		int sz = dcel->getSize();
		int ret = -1;
		float min = 100000.0;
		for(int i = 0; i < sz; ++i) {
			float t = Utilities::dist2(newPoint, dcel->getFace(i)->getSite());
			if(min > t) {
				ret = i;
				min = t;
			}
		}
		return ret;
	}
	void addNewSite(Point newSite, DCEL& vor)
	{
		if(vor.getNumberOfFaces()==0)
		{
			Face* f = new Face(newSite);

			U->setIncidentFace(f);
			R->setIncidentFace(f);
			D->setIncidentFace(f);
			L->setIncidentFace(f);

			f->setIndicidentEdge(D);

			vor.addFace(f);
			return;
		}
		/*
		//if(vor.getNumberOfFaces()==1)
		//{
		//	Face* f1 = vor.getFace(0);
		//	Face* f2 = new Face(newSite);

		//	vor.addFace(f2);

		//	//F1 is on h's left

		//	Line L = getPerpendicular2Points(newSite, f1->getSite());
		//	Point I[2];

		//	HalfEdge *tmp = U, *A[2];
		//	int c = 0;

		//	do {
		//		if(Utilities::computeSegmentIntersection(tmp->getStart(), tmp->getEnd(), L.start, L.end, I[c])) {
		//			A[c] = tmp;
		//			++c;
		//		}
		//		tmp = tmp->getNext();
		//	} while(tmp != U);

		//	HalfEdge *h, *t;
		//	if(Utilities::crossProduct(L.start, L.end, newSite) < 0)
		//	{
		//		h = new HalfEdge(L.start, L.end);
		//		t = new HalfEdge(L.end, L.start);
		//	}
		//	else
		//	{
		//		h = new HalfEdge(L.end, L.start);
		//		t = new HalfEdge(L.start, L.end);
		//	}


		//	h->setTwin(t);				t->setTwin(h);

		//	h->setIncidentFace(f1);		f1->setIndicidentEdge(h);

		//	t->setIncidentFace(f2);		f2->setIndicidentEdge(t);
		//	return;
		//}
		*/
		else {

			int idx = getNearestFace(newSite, &vor), c = 0;
			Face* f1 = vor.getFace(idx);
			Face* f2 = new Face(newSite);
			vor.addFace(f2);
			Point I[2];
			HalfEdge* A[2];
			Line L = getPerpendicular2Points(newSite, f1->getSite());
			HalfEdge* E = f1->getIncidentEdge();
			HalfEdge* E1 = E;
			do {
				if(c == 2) break;
				if(Utilities::computeSegmentIntersection(E1->getStart(), E1->getEnd(), L.start, L.end, I[c])) {
					A[c] = E1;
					++c;
				}
				E1 = E1->getNext();
			} while(E1 != E);


			HalfEdge *h, *t;

			if(Utilities::crossProduct(I[0], I[1], newSite) < 0)
				swap(A[0], A[1]), swap(I[0], I[1]);
			h = new HalfEdge(I[0], I[1]);
			t = new HalfEdge(I[1], I[0]);
			DrawHalfEdge(h, "h");
			DrawHalfEdge(t, "t");
			h->setTwin(t);				t->setTwin(h);

			h->setIncidentFace(f2);		f2->setIndicidentEdge(h);
			t->setIncidentFace(f1);		f1->setIndicidentEdge(t);

			HalfEdge *q1, *q2, *p1, *p2;
			q1 = new HalfEdge(A[0]->getStart(), I[0]);
			q2 = new HalfEdge(I[0], A[0]->getEnd());
			p1 = new HalfEdge(A[1]->getStart(), I[1]);
			p2 = new HalfEdge(I[1], A[1]->getEnd());

			DrawHalfEdge(q1, "q1");
			DrawHalfEdge(q2, "q2");
			DrawHalfEdge(p1, "p1");
			DrawHalfEdge(p2, "p2");

			q1->setPrevious(A[0]);
			q1->setNext(h);
			h->setPrevious(q1);
			q1->setIncidentFace(f2);
			q2->setPrevious(t);
			q2->setNext(A[0]->getNext());
			q2->setIncidentFace(f1);

			p1->setNext(t);
			t->setPrevious(p1);
			p1->setPrevious(A[1]->getPrevious());
			p1->setIncidentFace(f1);
			h->setIncidentFace(f1); f1->setIndicidentEdge(h);
			t->setIncidentFace(f2); f2->setIndicidentEdge(t);

			Point source = h->getStart(), target(0, 0);
			while(source != target) {

				f1 = A[1]->getTwin()->getIncidentFace();
				Line L = getPerpendicular2Points(newSite, f1->getSite());
				E = f1->getIncidentEdge();
				E1 = E;
				c = 0;
				do {
					if(c == 2) break;
					if(Utilities::computeSegmentIntersection(E1->getStart(), E1->getEnd(), L.start, L.end, I[c])) {
						A[c] = E1;
						++c;
					}
					E1 = E1->getNext();
				} while(E1 != E);

				if(Utilities::crossProduct(I[0], I[1], newSite) < 0)
					swap(A[0], A[1]), swap(I[0], I[1]);
				h = new HalfEdge(I[0], I[1]);
				t = new HalfEdge(I[1], I[0]);
				
				DrawHalfEdge(h, "h");
				DrawHalfEdge(t, "t");
				h->setTwin(t);				t->setTwin(h);

				h->setIncidentFace(f2);		f2->setIndicidentEdge(h);
				t->setIncidentFace(f1);		f1->setIndicidentEdge(t);

				
				q1 = new HalfEdge(A[0]->getStart(), I[0]);
				q2 = new HalfEdge(I[0], A[0]->getEnd());
				p1 = new HalfEdge(A[1]->getStart(), I[1]);
				p2 = new HalfEdge(I[1], A[1]->getEnd());

				DrawHalfEdge(q1, "q1");
				DrawHalfEdge(q2, "q2");
				DrawHalfEdge(p1, "p1");
				DrawHalfEdge(p2, "p2");

				q1->setPrevious(A[0]);
				q1->setNext(h);
				h->setPrevious(q1);
				q1->setIncidentFace(f2);
				q2->setPrevious(t);
				q2->setNext(A[0]->getNext());
				q2->setIncidentFace(f1);

				p1->setNext(t);
				t->setPrevious(p1);
				p1->setPrevious(A[1]->getPrevious());
				p1->setIncidentFace(f1);
				h->setIncidentFace(f1); f1->setIndicidentEdge(h);
				t->setIncidentFace(f2); f2->setIndicidentEdge(t);

				target = h->getEnd();
			}

		}
	}
	void run(const vector<Point>& inputPoints, const vector<Line>& inputLines,
		vector<Point>& outputPoints, vector<Line>& outputLines)
	{
		//map<Face*, int> ff;
		//Face* a = new Face(Point(1,2));
		//Face* b = new Face(Point(2,3));
		//ff[a] = 1;
		//ff[b] = 2;
		//ff[b] = 1;
		//glClearColor(0,0,0,1);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glFlush();
		DCEL voronoiDCEL;
		if(inputLines.size()<4)
			return;
		U = new HalfEdge(inputLines[0].start, inputLines[0].end);
		R = new HalfEdge(inputLines[1].start, inputLines[1].end);
		D = new HalfEdge(inputLines[2].start, inputLines[2].end);
		L = new HalfEdge(inputLines[3].start, inputLines[3].end);

		U->setNext(R);	U->setPrevious(L);
		R->setNext(D);	L->setPrevious(D);
		D->setNext(L);	D->setPrevious(R);
		L->setNext(U);	R->setPrevious(U);

		for(int i = 0; i < inputPoints.size(); i ++)
			addNewSite(inputPoints[i], voronoiDCEL);
	}
};
#endif