#include "XmlWriter.h"

XmlWriter::XmlWriter()
{

}

void XmlWriter::setPipelineParameters(PipelineParameters* parameters) {m_parameters=parameters;}

void XmlWriter::writeElement(QXmlStreamWriter* stream, QString tag, QString name, QString value)
{
   stream->writeStartElement(tag);
   stream->writeAttribute(name, value);
   stream->writeEndElement();
}

void XmlWriter::writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2)
{
   stream->writeStartElement(tag);
   stream->writeAttribute(name1, value1);
   stream->writeAttribute(name2, value2);
   stream->writeEndElement();
}

void XmlWriter::writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2, QString name3, QString value3)
{
   stream->writeStartElement(tag);
   stream->writeAttribute(name1, value1);
   stream->writeAttribute(name2, value2);
   stream->writeAttribute(name3, value3);
   stream->writeEndElement();
}

void XmlWriter::writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2, QString name3, QString value3, QString name4, QString value4)
{
   stream->writeStartElement(tag);
   stream->writeAttribute(name1, value1);
   stream->writeAttribute(name2, value2);
   stream->writeAttribute(name3, value3);
   stream->writeAttribute(name4, value4);
   stream->writeEndElement();
}

void XmlWriter::writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2, QString name3, QString value3, QString name4, QString value4, QString name5, QString value5)
{
   stream->writeStartElement(tag);
   stream->writeAttribute(name1, value1);
   stream->writeAttribute(name2, value2);
   stream->writeAttribute(name3, value3);
   stream->writeAttribute(name4, value4);
   stream->writeAttribute(name5, value5);
   stream->writeEndElement();
}

void XmlWriter::writeDataConfiguration(QString file_path)
{
   QFile* file = new::QFile(file_path);
   file->open(QIODevice::WriteOnly);

   QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
   stream->setAutoFormatting(true);

   stream->writeStartDocument(); 
   stream->writeDTD("<!DOCTYPE Neoseg-pipeline-data>");

   writeData(stream); 

   stream->writeEndDocument();

   file->close(); 
}

void XmlWriter::writeData(QXmlStreamWriter* stream)
{
   stream->writeStartElement("Data");
   stream->writeStartElement("Inputs");
   writeElement(stream, "T1", "path", m_parameters->getT1());
   writeElement(stream, "T2", "path", m_parameters->getT2());
   writeElement(stream, "Mask", "path", m_parameters->getMask());
   writeElement(stream, "DWI", "path", m_parameters->getDWI());
   stream->writeEndElement();
   stream->writeStartElement("Outputs");
   writeElement(stream, "Output-files","prefix", m_parameters->getPrefix(), "suffix", m_parameters->getSuffix());
   writeElement(stream, "Output-directory", "path", m_parameters->getOutput());
   stream->writeEndElement();
   stream->writeEndElement();
}

void XmlWriter::writeGeneralParameters(QXmlStreamWriter* stream)
{
    stream->writeStartElement("General-parameters");
    writeElement(stream, "New-atlas-computation", "bool", QString::number(m_parameters->getNewAtlas()));

   if(m_parameters->getNewAtlas())
   {
      stream->writeStartElement("New-atlas-parameters");

      writeElement(stream, "Atlas-population-directory","path", m_parameters->getAtlasPopulationDirectory());

      stream->writeStartElement("Atlas-Population");
      QStringList atlases = m_parameters->getSelectedAtlases();
      QStringList::const_iterator it;
      for (it = atlases.constBegin(); it != atlases.constEnd(); ++it)      
      {
         writeElement(stream, "Atlas", "name", *it);         
      }
      stream->writeEndElement(); // ATLAS-POPULATION

      writeElement(stream, "Smoothing","type", m_parameters->getSmoothing(), "size", QString::number(m_parameters->getSmoothingSize()));    

      writeElement(stream, "Computing-weights", "bool", QString::number(m_parameters->getComputingWeights()),"modality", m_parameters->getWeightsModality(), "radius", QString::number(m_parameters->getWeightsRadius()), "unit", m_parameters->getWeightsRadiusUnit());
      writeElement(stream, "Including-FA", "bool", QString::number(m_parameters->getIncludingFA()), "shift", QString::number(m_parameters->getFAShift()), "sigma", QString::number(m_parameters->getFASigmaScale()), "weight", QString::number(m_parameters->getFAWeight()), "smoothing-size", QString::number(m_parameters->getFASmoothingSize()));  
      stream->writeEndElement(); // NEW-ATLAS
   }

   else
   {
      writeElement(stream, "Existing-atlas","path", m_parameters->getExistingAtlas());
   }

   writeElement(stream, "Neoseg-images","using-FA", QString::number(m_parameters->getUsingFA()), "using-AD", QString::number(m_parameters->getUsingAD()));   

   writeElement(stream, "Computing-3-labels-seg", "bool", QString::number(m_parameters->getComputing3LabelsSeg()));

   writeElement(stream, "Reassigning-white-matter", "bool", QString::number(m_parameters->getReassigningWhiteMatter()), "size-threshold", QString::number(m_parameters->getSizeThreshold()));

   stream->writeStartElement("Computation-parameters");  
   writeElement(stream, "Overwriting","bool", QString::number(m_parameters->getOverwriting()));
   writeElement(stream, "Stopping-if-error","bool", QString::number(m_parameters->getStoppingIfError()));
   writeElement(stream, "Cleaning-up","bool", QString::number(m_parameters->getCleaningUp()));
   writeElement(stream, "Computing-system","name", m_parameters->getComputingSystem());
   stream->writeEndElement(); // COMPUTATION
   stream->writeStartElement("Preprocessing");  
   writeElement(stream, "Skull-strip", "bool", QString::number(m_parameters->getSkullStripping()));
   writeElement(stream, "Correct-inhomogeneity", "bool", QString::number(m_parameters->getCorrecting()));
   stream->writeEndElement(); // Preprocessing
   stream->writeEndElement(); // GENERAL-PARAMETERS
}

void XmlWriter::writeAntsParameters(QXmlStreamWriter* stream, AntsParameters* antsParameters)
{
   stream->writeStartElement("ANTS-parameters-" + antsParameters->getName());

   writeElement(stream, "First-modality", "metric", antsParameters->getImageMetric1(), "weight", QString::number(antsParameters->getWeight1()), "radius", QString::number(antsParameters->getRadius1()));
   writeElement(stream, "Second-modality", "metric", antsParameters->getImageMetric2(), "weight", QString::number(antsParameters->getWeight2()), "radius", QString::number(antsParameters->getRadius2()));

   writeElement(stream, "Iterations", "J",  QString::number(antsParameters->getIterationsJ()), "K",  QString::number(antsParameters->getIterationsK()), "L",  QString::number(antsParameters->getIterationsL()));

   writeElement(stream, "Transformation", "type", antsParameters->getTransformationType(), "gradient-step-length", QString::number(antsParameters->getGradientStepLength()), "number-of-time-steps", QString::number(antsParameters->getNumberOfTimeSteps()), "delta-time", QString::number(antsParameters->getDeltaTime()));

   writeElement(stream, "Regularization", "type", antsParameters->getRegularizationType(), "gradient-field-sigma", QString::number(antsParameters->getGradientFieldSigma()), "deformation-field-sigma", QString::number(antsParameters->getDeformationFieldSigma()));

   writeElement(stream, "Mask", "brain-mask", QString::number(antsParameters->getUsingMask()), "smoothed-brain-mask", QString::number(antsParameters->getUsingSmoothedMask()));
   writeElement(stream, "Resources", "Number-of-Registrations", QString::number(antsParameters->getNumberOfRegistrations()), "Number-of-Cores", QString::number(antsParameters->getNumberOfCores()), "Number-of-GB", QString::number(antsParameters->getNumberOfGB()));

   stream->writeEndElement();
}

void XmlWriter::writeNeosegParameters(QXmlStreamWriter* stream)
{
   NeosegParameters* neosegParameters=m_parameters->getNeosegParameters();
   stream->writeStartElement("Neoseg-parameters");

   writeElement(stream, "Reference-image", "name", neosegParameters->getReferenceImage()); 

   writeElement(stream, "Filtering", "type", neosegParameters->getFilterMethod(), "iterations", QString::number(neosegParameters->getNumberOfIterations()), "time-step", QString::number(neosegParameters->getTimeStep())); 
   writeElement(stream, "Prior-threshold", "value", QString::number(neosegParameters->getPriorThreshold()));

   writeElement(stream, "Mas-bias-degree", "value", QString::number(neosegParameters->getMaxBiasDegree())); 

   writeElement(stream, "Priors", "prior1", QString::number(neosegParameters->getPrior1()),\
                                  "prior2", QString::number(neosegParameters->getPrior2()),\
                                  "prior3", QString::number(neosegParameters->getPrior3()),\
                                  "prior4", QString::number(neosegParameters->getPrior4()),\
                                  "prior5", QString::number(neosegParameters->getPrior5()));    
  
   writeElement(stream, "Refinement", "bool", QString::number(neosegParameters->getRefinement()), "initial-parzen-kernel-width", QString::number(neosegParameters->getInitialParzenKernelWidth())); 

   stream->writeEndElement();
}


void XmlWriter::writeParametersConfiguration(QString file_path)
{
   QFile* file = new::QFile(file_path);
   file->open(QIODevice::WriteOnly);

   QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
   stream->setAutoFormatting(true);

   stream->writeStartDocument(); 

   stream->writeDTD("<!DOCTYPE Neoseg-pipeline-parameters>");

   stream->writeStartElement("Parameters");

   writeGeneralParameters(stream);  
   writeAntsParameters(stream, m_parameters->getAntsParametersDTI());
   writeAntsParameters(stream, m_parameters->getAntsParametersAtlas());
   writeNeosegParameters(stream); 

   stream->writeEndElement();

   stream->writeEndDocument();

   file->close(); 
}

void XmlWriter::writeExecutables(QXmlStreamWriter* stream)
{
   ExecutablePaths* executablePaths = m_parameters->getExecutablePaths();
   stream->writeStartElement("Tools-Paths");
   stream->writeStartElement("Executables");
   writeElement(stream, "python", "path", executablePaths->getExecutablePath("python"));
   writeElement(stream, "SegPostProcessCLP", "path", executablePaths->getExecutablePath("SegPostProcessCLP")); 
   writeElement(stream, "N4ITKBiasFieldCorrection", "path", executablePaths->getExecutablePath("N4ITKBiasFieldCorrection")); 
   writeElement(stream, "ANTS", "path", executablePaths->getExecutablePath("ANTS")); 
   writeElement(stream, "ITKTransformTools", "path", executablePaths->getExecutablePath("ITKTransformTools")); 
   writeElement(stream, "dtiestim", "path", executablePaths->getExecutablePath("dtiestim")); 
   writeElement(stream, "dtiprocess", "path", executablePaths->getExecutablePath("dtiprocess")); 
   writeElement(stream, "bet2", "path", executablePaths->getExecutablePath("bet2")); 
   writeElement(stream, "SpreadFA", "path", executablePaths->getExecutablePath("SpreadFA"));
   writeElement(stream, "unu", "path", executablePaths->getExecutablePath("unu")); 
   writeElement(stream, "ResampleScalarVectorDWIVolume", "path", executablePaths->getExecutablePath("ResampleScalarVectorDWIVolume")); 
   writeElement(stream, "ImageMath", "path", executablePaths->getExecutablePath("ImageMath"));
   writeElement(stream, "WeightedLabelsAverage", "path", executablePaths->getExecutablePath("WeightedLabelsAverage")); 
   writeElement(stream, "neoseg", "path", executablePaths->getExecutablePath("neoseg"));
   writeElement(stream, "ReassignWhiteMatter", "path", executablePaths->getExecutablePath("ReassignWhiteMatter"));
   writeElement(stream, "InsightSNAP", "path", executablePaths->getExecutablePath("InsightSNAP"));
   stream->writeEndElement(); 

   LibraryPaths* libraryPaths = m_parameters->getLibraryPaths();

   stream->writeStartElement("Library-directories"); 
   writeElement(stream, "FSL", "path", libraryPaths->getLibraryPath("FSL")); 
   stream->writeEndElement(); 
   stream->writeEndElement(); 
}

void XmlWriter::writeExecutablesConfiguration(QString file_path)
{
   QFile* file = new::QFile(file_path);
   file->open(QIODevice::WriteOnly);

   QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
   stream->setAutoFormatting(true);

   stream->writeStartDocument(); 
   stream->writeDTD("<!DOCTYPE Neoseg-pipeline-executables>");

   writeExecutables(stream); 

   stream->writeEndDocument();

   file->close(); 
}




