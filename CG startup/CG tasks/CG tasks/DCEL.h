#pragma once
#ifndef DCEL_H
#define DCEL_H
#include "Utilities.h"
class Face;
class HalfEdge
{
private:
	Point		Start;
	Point		End;
	HalfEdge	*Next;
	HalfEdge	*Prev;
	HalfEdge	*Twin;
	Face*		IncidentFace;

	HalfEdge(){}
public:
	HalfEdge(Point start, Point end)	{ Start = start, End = end; }
	void setIncidentFace(Face* f)		{ IncidentFace = f;			}
	void setNext(HalfEdge* next)		{ Next = next;				}
	void setPrevious(HalfEdge* prev)	{ Prev = prev;				}
	void setTwin(HalfEdge* twin)		{ Twin = twin;				}
	void setStart(Point s)				{ Start = s;				}
	void setEnd(Point e)				{ End = e;					}



	Face*		getIncidentFace()	const { return IncidentFace;	}
	HalfEdge*	getNext()			const { return Next;			}
	HalfEdge*	getPrevious()		const { return Prev;			}
	HalfEdge*	getTwin()			const { return Twin;			}
	Line		getEdge()			const { return Line(Start,End);	}
	Point		getStart()			const { return Start;			}
	Point		getEnd()			const { return End;				}

	bool operator == (const HalfEdge E) const {
		return (Start == E.Start && End == E.End);
	}
};
class Face
{
private:
	HalfEdge*	IncidentEdge;
	Point		Site;

	Face(){}
public:
	Face(Point site)						{ Site = site; }
	Face(Point site, HalfEdge incidentEdge)	{ Site = site, IncidentEdge = &incidentEdge; }
	void setIndicidentEdge(HalfEdge* e)		{ IncidentEdge = e; }
	
	Point		getSite()			const	{ return Site; }
	HalfEdge*	getIncidentEdge()	const	{ return IncidentEdge; }
};
class DCEL
{
	vector<Face*> Faces;
public:
	unsigned getNumberOfFaces()const {return Faces.size(); }
	//vector<HalfEdge*> HalfEdges;
	void	addFace(Face* f)			{Faces.push_back(f);}
	Face*	getFace(int index)	const	{ return Faces[index];}
	int getSize()				const	{ return Faces.size();}
};
#endif