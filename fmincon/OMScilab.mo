model OMScilab

  function Scilab_Operation 
    input String objective_function;
	input String x0;
	input String A;
	input String b;
	input String Aeq;
	input String beq;
	input String lb;
	input String ub;
	input String optimization_function;
    output Real[3] results;
      external "C" annotation(Library={"call_scilab_from_c.o","C:/scilab-6.1.1/bin/call_scilab.dll","C:/scilab-6.1.1/bin/api_scilab.dll"}, LibraryDirectory="modelica://OMScilab"); 
  end Scilab_Operation;

  Real[3] results; String objective_function, x0, A, b, Aeq, beq, lb, ub, optimization_function;
  algorithm
    objective_function := "x(1)^2 - x(1)*x(2)/3 + x(2)^2";
	x0 := "[0,0]";
	A := "[]";
	b := "[]";
	Aeq := "[]";
	beq := "[]";
	lb := "[]";
	ub := "[]";
	optimization_function := "fot_fmincon";
	results := Scilab_Operation(objective_function, x0, A, b, Aeq, beq, lb, ub, optimization_function);

end OMScilab;