/**
 * \file face.hpp
 *
 * \brief Definition of the class Face, which represents a face (i.e.,
 * a  triangle)  from  a  surface  mesh represented  by  a  DCEL  data
 * structure.
 *
 * \author
 * Marcelo Ferreira Siqueira \n
 * Universidade Federal do Rio Grande do Norte, \n
 * Departamento de Informatica e Matematica Aplicada, \n
 * marcelo at dimap (dot) ufrn (dot) br
 *
 * \version 2.0
 * \date January 2010
 *
 * \attention This program is distributed WITHOUT ANY WARRANTY, and it
 *            may be freely redistributed under the condition that the
 *            copyright notices  are not removed,  and no compensation
 *            is received. Private, research, and institutional use is
 *            free. Distribution of this  code as part of a commercial
 *            system  is permissible ONLY  BY DIRECT  ARRANGEMENT WITH
 *            THE AUTHOR.
 */

#ifndef FACE_HPP
#define FACE_HPP

#include "halfedge.h"   // Halfedge


/**
 * \defgroup DCELNameSpace Namespace dcel.
 * @{
 */

/**
 * \namespace dcel
 *
 * \brief   The   namespace   dcel   contains   the   definition   and
 * implementation  of  all  classes   of  the  data  structure  Doubly
 * Connected Edge List (DCEL), which  can be used to represent surface
 * meshes with empty boundary.
 *
 */

namespace dcel {

  /**
   * Forward definition of classes.
   */
  template <
             typename VAttrib ,
             typename FAttrib ,
             typename EAttrib ,
             typename HAttrib
           >
  class Surface ;


  /**
   * \class Face
   *
   * \brief This  class represents  a face (i.e.,  a triangle)  from a
   * surface mesh represented by the DCEL data structure.
   */
  template <
             typename VAttrib = int,
             typename FAttrib = int,
             typename EAttrib = int,
             typename HAttrib = int
           >
  class Face {
  public:
    // ---------------------------------------------------------------
    //
    // Type definitions
    //
    // ---------------------------------------------------------------

   /**
     * \typedef Surface
     *
     * \brief Defines Surface as  an alias for dcel::Surface< VAttrib,
     * FAttrib , EAttrib , HAttrib >.
     */
    typedef dcel::Surface< VAttrib, FAttrib , EAttrib , HAttrib >
      Surface ;

    /**
     * \typedef Halfedge
     *
     * \brief  Defines  Halfedge   as  an  alias  for  dcel::Halfedge<
     * VAttrib, FAttrib , EAttrib , HAttrib >.
     */
    typedef dcel::Halfedge< VAttrib, FAttrib , EAttrib , HAttrib >
      Halfedge ;

    /**
     * \typedef Vertex
     *
     * \brief  Defines  Vertex   as  an  alias  for  dcel::Vertex<
     * VAttrib, FAttrib , EAttrib , HAttrib >.
     */
    typedef dcel::Vertex< VAttrib, FAttrib , EAttrib , HAttrib >
      Vertex ;

    // ---------------------------------------------------------------
    //
    // Public methods
    //
    // ---------------------------------------------------------------

    /**
     * \fn Face( Halfedge* h )
     *
     * \brief Creates an instance of this class.
     *
     * \param h A pointer to the first halfedge of this face.
     */
    Face( Halfedge* h ) : _surface( 0 ) , _halfedge( h )
    {}


    /**
     * \fn Face( Surface* s , Halfedge* h )
     *
     * \brief Creates an instance of this class.
     *
     * \param s A pointer to the surface this face belongs to.
     * \param h A pointer to the first halfedge of this face.
     */
    Face( Surface* s , Halfedge* h ) : _surface( s ) , _halfedge( h )
    {}


    /**
     * \fn ~Face()
     *
     * \brief Destroys an instance of this class.
     */
    ~Face()
    {
      set_halfedge( 0 ) ;
    }


    /**
     * \fn Surface* get_surface() const
     *
     * \brief Returns a pointer to the surface this face belongs to.
     *
     * \return A pointer to the face this face belongs to.
     */
    Surface* get_surface() const
    {
      return _surface ;
    }


    /**
     * \fn Halfedge* get_halfedge() const
     *
     * \brief Returns a pointer to the first half-edge of this face.
     *
     * \return A pointer to the first half-edge of this face.
     */
    Halfedge* get_halfedge() const
    {
      return _halfedge ;
    }


    /**
     * \fn void set_surface( Surface* s )
     *
     * \brief Assigns  an address to  the pointer to the  surface this
     * face belongs to.
     *
     * \param s The address of the surface this face belongs to.
     */
    void set_surface( Surface* s )
    {
      _surface = s ;
    }


    /**
     * \fn void set_halfedge( Halfedge* h )
     *
     * \brief Assigns an address to the pointer to the first half-edge
     * of this face.
     *
     * \param h The address of the first half-edge of this face.
     */
    void set_halfedge( Halfedge* h )
    {
      _halfedge = h ;
    }


    /**
     * \fn FAttrib& get_attributes()
     *
     * \brief Returns the set of attributes associated with this face.
     *
     * \return A reference to the set of attributes associated with this
     * face.
     */
    FAttrib& get_attributes()
    {
      return _attributes ;
    }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//This code was added by Francisco Fernandes (in 29/05/2011)

    /**
    * \fn double* face_normal(Face *f)
    *
    * \brief Calculates a face non-normalized normal.
    *
    * \param f Face which the normal will be calculated.
    *
    * \return An array with the face normal coordinates.
    */

    double* face_normal()
    {
        //
        // Get the face halfedge to retrieve the vertices.
        //
        Halfedge *h1 = get_halfedge();

        double *norm = new double[3];

        Vertex *v1 = h1->get_origin();
        Vertex *v2 = h1->get_next()->get_origin();
        Vertex *v3 = h1->get_prev()->get_origin();

        //
        // Calculate the
        //
        double vec1[3] = { ( v2->x() - v1->x() ), ( v2->y() - v1->y() ), ( v2->z() - v1->z() ) }; //b
        double vec2[3] = { ( v3->x() - v1->x() ), ( v3->y() - v1->y() ), ( v3->z() - v1->z() ) }; //a



        norm[0] = vec2[1]*vec1[2] - vec2[2]*vec1[1];
        norm[1] = vec2[2]*vec1[0] - vec2[0]*vec1[2];
        norm[2] = vec2[0]*vec1[1] - vec2[1]*vec1[0];

        return norm;
    }


    /**
    * \fn double face_area(Face *f)
    *
    * \brief Calculates a face area using the Herom's formula.
    *
    * \param f Face which the area will be calculated.
    *
    * \return The face area.
    */

    double face_area( )
    {
        //
        // Get the face halfedge to retrieve the vertices.
        //

        Halfedge *h1 = get_halfedge();

        Vertex *v1 = h1->get_origin();
        Vertex *v2 = h1->get_next()->get_origin();
        Vertex *v3 = h1->get_prev()->get_origin();

        //
        // Calculate the face vectors and norms.
        //

        double vec1[3] = { ( v2->x() - v1->x() ), ( v2->y() - v1->y() ), ( v2->z() - v1->z() ) };
        double vec2[3] = { ( v3->x() - v2->x() ), ( v3->y() - v2->y() ), ( v3->z() - v2->z() ) };
        double vec3[3] = { ( v1->x() - v3->x() ), ( v1->y() - v3->y() ), ( v1->z() - v3->z() ) };

        double nv1 = sqrt(vec1[0]*vec1[0] + vec1[1]*vec1[1] + vec1[2]*vec1[2]);
        double nv2 = sqrt(vec2[0]*vec2[0] + vec2[1]*vec2[1] + vec2[2]*vec2[2]);
        double nv3 = sqrt(vec3[0]*vec3[0] + vec3[1]*vec3[1] + vec3[2]*vec3[2]);

        //
        //Semi-perimeter and area calculation
        //

        double s = ( nv1 + nv2 + nv3 ) / 2 ;

        return sqrt( s*( s - nv1 )*( s - nv2 )*( s - nv3 ) ) ;
    }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


  private:
    // ---------------------------------------------------------------
    //
    // Private data members
    //
    // ---------------------------------------------------------------

    Surface* _surface ;   ///< Pointer to the surface this face belongs to.

    Halfedge* _halfedge ;   ///< Pointer to the first half-edge of the half-edge cycle of this face.

    FAttrib _attributes ;   ///< Set of attributes associated with this face.

  } ;

}

/** @} */ //end of group class.

#endif   // FACE_HPP

