/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#ifndef __itktubeMergeAdjacentImagesFilter_h
#define __itktubeMergeAdjacentImagesFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

namespace tube
{

template< typename TImage >
class MergeAdjacentImagesFilter :
  public ImageToImageFilter< TImage, TImage >
{
public:

  typedef MergeAdjacentImagesFilter                          Self;
  typedef ImageToImageFilter< TInputImage, OutputImageType > Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;

  typedef TImage                                             ImageType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( MergeAdjacentImagesFilter, ImageToImageFilter );

  /** Set input image 1 */
  void SetInput1(const ImageType * image);

  /** Set input image 2 */
  void SetInput2(const ImageType * image);

  /** Set value used for output pixels that dont intersect with input image */
  itkSetMacro( Background, typename TImage::PixelType );

  /** Set value used for output pixels that dont intersect with input image */
  itkGetMacro( Background, typename TImage::PixelType );

  /** Set if zero-valued input pixels should be ignored */
  itkSetMacro( MaskZero, bool );

  /** Get if zero-valued input pixels should be ignored */
  itkGetMacro( MaskZero, bool );

  /** Set number of registration iterations */
  itkSetMacro( MaxIterations, unsigned int );

  /** Get number of registration iterations */
  itkGetMacro( MaxIterations, unsigned int );

protected:
  MergeAdjacentImagesFilter( void );
  virtual ~MergeAdjacentImagesFilter( void ) {}

  virtual void GenerateData();

  void PrintSelf( std::ostream & os, Indent indent ) const;

private:
  // Purposely not implemented
  MergeAdjacentImagesFilter( const Self & );
  void operator = ( const Self & );

  typename TImage::PixelType             m_Background;
  bool                                   m_MaskZero;
  unsigned int                           m_MaxIterations;

};  // End class MergeAdjacentImagesFilter

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeMergeAdjacentImagesFilter.hxx"
#endif

#endif
