/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFilterWatcher.h, v $
  Language:  C++
  Date:      $Date: 2007-01-29 14:42:11 $
  Version:   $Revision: 1.15 $

  Copyright ( c ) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFilterWatcher_h
#define __itkFilterWatcher_h

#include <ctime>

#include <itkCommand.h>
#include <itkProcessObject.h>

namespace itk
{

// The following class is a convenience  to watch the progress of a filter

class FilterWatcher
{

public:

  FilterWatcher(itk::ProcessObject* o, const char *comment="")
    {
    m_Start = 0;
    m_End = 0;
    m_Process = o;
    m_Steps = 0;
    m_Comment = comment;
    m_TestAbort = false;
#if defined(_COMPILER_VERSION) && (_COMPILER_VERSION == 730)
    m_Quiet = true;
#else
    m_Quiet = false;
#endif
    itk::SimpleMemberCommand<FilterWatcher>::Pointer startFilterCommand;
    itk::SimpleMemberCommand<FilterWatcher>::Pointer endFilterCommand;
    itk::SimpleMemberCommand<FilterWatcher>::Pointer progressFilterCommand;
    itk::SimpleMemberCommand<FilterWatcher>::Pointer iterationFilterCommand;
    itk::SimpleMemberCommand<FilterWatcher>::Pointer abortFilterCommand;

    startFilterCommand =    itk::SimpleMemberCommand<FilterWatcher>::New();
    endFilterCommand =      itk::SimpleMemberCommand<FilterWatcher>::New();
    progressFilterCommand = itk::SimpleMemberCommand<FilterWatcher>::New();
    iterationFilterCommand = itk::SimpleMemberCommand<FilterWatcher>::New();
    abortFilterCommand = itk::SimpleMemberCommand<FilterWatcher>::New();

    startFilterCommand->SetCallbackFunction(this,
                                          &FilterWatcher::StartFilter);
    endFilterCommand->SetCallbackFunction(this,
                                          &FilterWatcher::EndFilter);
    progressFilterCommand->SetCallbackFunction(this,
                                          &FilterWatcher::ShowProgress);
    iterationFilterCommand->SetCallbackFunction(this,
                                          &FilterWatcher::ShowIteration);
    abortFilterCommand->SetCallbackFunction(this,
                                          &FilterWatcher::ShowAbort);
    m_Process->AddObserver(itk::StartEvent(), startFilterCommand);
    m_Process->AddObserver(itk::EndEvent(), endFilterCommand);
    m_Process->AddObserver(itk::ProgressEvent(), progressFilterCommand);
    m_Process->AddObserver(itk::IterationEvent(), iterationFilterCommand);
    m_Process->AddObserver(itk::AbortEvent(), abortFilterCommand);
    }

  virtual ~FilterWatcher( void ) {}

  virtual void ShowProgress( void )
    {
    m_Steps++;
    if(!m_Quiet)
      {
      std::cout << " | " << m_Process->GetProgress() << std::flush;
      if((m_Steps % 10) == 0)
        {
        std::cout << std::endl;
        }
      }
    if(m_TestAbort)
      {
      if(m_Process->GetProgress() > .03)
        {
        m_Process->AbortGenerateDataOn();
        }
      }
    }

  virtual void ShowAbort( void )
    {
    std::cout << std::endl << "      ABORT" << std::endl << std::flush;
    }

  virtual void ShowIteration( void )
    {
    std::cout << " # " << std::flush;
    m_Iterations++;
    }

  virtual void StartFilter( void )
    {
    m_Steps = 0;
    m_Iterations = 0;
    m_Start = std::clock();
    std::cout << "-------- Start " << m_Process->GetNameOfClass()
              << " \"" << m_Comment << "\" "
              << m_Process
              << (m_Quiet ? "Progress Quiet " : "Progress ")
              << std::flush;
    }

  const char *GetNameOfClass ( void ) {return m_Process->GetNameOfClass();}
  virtual void EndFilter( void )
    {
    m_End = std::clock();
    std::cout << std::endl << "Filter took "
              << static_cast<double>(m_End - m_Start) / CLOCKS_PER_SEC
              << " seconds.";
    std::cout << std::endl << std::endl
              << "-------- End " << m_Process->GetNameOfClass()
              << " \"" << m_Comment << "\" "
              << m_Process << std::flush;
    if(m_Steps < 1)
      {
      itkExceptionMacro ("Filter does not have progress.");
      }
    }

  void QuietOn( void ) {m_Quiet = true;}
  void QuietOff( void ) {m_Quiet = false;}
  void TestAbortOn( void ) {m_TestAbort = true;}
  void TestAbortOff( void ) {m_TestAbort = false;}

protected:

  std::clock_t                m_Start;
  std::clock_t                m_End;
  int                         m_Steps;
  int                         m_Iterations;
  bool                        m_Quiet;
  bool                        m_TestAbort;
  std::string                 m_Comment;
  itk::ProcessObject::Pointer m_Process;

private:

  FilterWatcher( void ); // Purposely not implemented

}; // End class FilterWatcher

} // End namespace itk

#endif // End !defined(__itkFilterWatcher_h)