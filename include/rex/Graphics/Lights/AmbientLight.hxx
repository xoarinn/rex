#pragma once

#include "Light.hxx"

REX_NS_BEGIN

/// <summary>
/// Defines an ambient light.
/// </summary>
class AmbientLight : public Light
{
    Color  _color;
    real32 _radianceScale;

public:
    /// <summary>
    /// Creates a new ambient light.
    /// </summary>
    __device__ AmbientLight();

    /// <summary>
    /// Creates a new ambient light.
    /// </summary>
    /// <param name="color">The ambient light's color.</param>
    /// <param name="ls">The ambient light's radiance scale.</param>
    __device__ AmbientLight( const Color& color, real32 ls );

    /// <summary>
    /// Destroys this ambient light.
    /// </summary>
    __device__ virtual ~AmbientLight();

    /// <summary>
    /// Gets this ambient light's color.
    /// </summary>
    __device__ const Color& GetColor() const;

    /// <summary>
    /// Gets this ambient light's radiance scale.
    /// </summary>
    __device__ real32 GetRadianceScale() const;

    /// <summary>
    /// Gets the direction of the incoming light at a hit point.
    /// </summary>
    /// <param name="sp">The shading point information containing hit data.</param>
    __device__ virtual vec3 GetLightDirection( ShadePoint& sp ) const;

    /// <summary>
    /// Gets the incident radiance at a hit point.
    /// </summary>
    /// <param name="sp">The shading point information containing hit data.</param>
    __device__ virtual Color GetRadiance( ShadePoint& sp ) const;

    /// <summary>
    /// Checks to see if the given ray is in shadow when viewed from this light.
    /// </summary>
    /// <param name="ray">The ray to check.</param>
    /// <param name="octree">The octree containing all of the geometry to check for.</param>
    /// <param name="sp">Current hit point information.</param>
    __device__ virtual bool IsInShadow( const Ray& ray, const ShadePoint& sp ) const;

    /// <summary>
    /// Sets whether or not this light should cast shadows.
    /// </summary>
    /// <param name="value">The new value.</param>
    __device__ virtual void SetCastShadows( bool value );

    /// <summary>
    /// Sets this ambient light's color.
    /// </summary>
    /// <param name="color">The new color.</param>
    __device__ void SetColor( const Color& color );

    /// <summary>
    /// Sets this ambient light's color.
    /// </summary>
    /// <param name="r">The new color's red component.</param>
    /// <param name="g">The new color's green component.</param>
    /// <param name="b">The new color's blue component.</param>
    __device__ void SetColor( real32 r, real32 g, real32 b );

    /// <summary>
    /// Sets this ambient light's radiance scale.
    /// </summary>
    /// <param name="ls">The new radiance scale.</param>
    __device__ void SetRadianceScale( real32 ls );
};

REX_NS_END