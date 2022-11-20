model OMScilab

  function Scilab_Operation 
    input String objective_function;
	input String starting_point;
	input String optimization_function;
    output Real[3] results;
      external "C" annotation(Library={"call_scilab_from_c.o","C:/scilab-6.1.1/bin/call_scilab.dll","C:/scilab-6.1.1/bin/api_scilab.dll"}, LibraryDirectory="modelica://OMScilab"); 
  end Scilab_Operation;

  Real[3] results; String objective_function, starting_point, optimization_function;
  algorithm
    objective_function := "0.6382*x(1)^2 + 0.3191*x(2)^2 - 0.2809*x(1)*x(2) - 67.906*x(1) - 14.29*x(2)";
    starting_point := "[100 200]";
	optimization_function := "fot_fminunc";
	results := Scilab_Operation(objective_function, starting_point, optimization_function);

end OMScilab;