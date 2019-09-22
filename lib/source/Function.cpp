#include "Function.h"
#include "Module.h"

spvgentwo::Function::Function(IAllocator* _pAllocator) :
	m_pModule(nullptr), List(_pAllocator),
	m_Function(_pAllocator),
	m_Parameters(_pAllocator),
	m_EntryPoint(_pAllocator),
	m_ExecutionModes(_pAllocator)
{
}

spvgentwo::Function::Function(Module* _pModule) :
	m_pModule(_pModule), List(_pModule->getAllocator()),
	m_Function(_pModule->getAllocator()),
	m_Parameters(_pModule->getAllocator()),
	m_EntryPoint(_pModule->getAllocator()),
	m_ExecutionModes(_pModule->getAllocator())
{
}

spvgentwo::Function::~Function()
{
}

void spvgentwo::Function::write(IWriter* _pWriter, spv::Id& _resultId)
{
	m_Function.write(_pWriter, _resultId);
	writeInstructions(_pWriter, m_Parameters, _resultId);

	for (BasicBlock& bb : *this)
	{
		bb.write(_pWriter, _resultId);
	}

	Instruction end(m_pAllocator, spv::Op::OpFunctionEnd);
	end.write(_pWriter, _resultId);
}

spvgentwo::Instruction* spvgentwo::Function::addParameter(Instruction* _pType)
{
	return m_Parameters.emplace_back(m_pAllocator).opFunctionParameter(_pType);;
}

bool spvgentwo::Function::finalize(Instruction* _pReturnType, const Flag<spv::FunctionControlMask> _control)
{
	if (m_pModule == nullptr || m_pFunctionType != nullptr)
	{
		return false;
	}

	// function signature type
	m_pFunctionType = m_pModule->compositeType(spv::Op::OpTypeFunction, _pReturnType);

	for (Instruction& param : m_Parameters)
	{
		m_pFunctionType->addOperand(param.getType());
	}

	m_Function.opFunction(_control, _pReturnType, m_pFunctionType);

	return true;
}

void spvgentwo::Function::promoteToEntryPoint(const spv::ExecutionModel _model, const char* _pEntryPointName)
{
	m_ExecutionModel = _model;
	m_pEntryPointName = _pEntryPointName;

	m_EntryPoint.reset();
	m_EntryPoint.opEntryPoint(_model, &m_Function, m_pEntryPointName);

	for(spvgentwo::Instruction* pVar : getGlobalVariableInterface())
	{
		m_EntryPoint.addOperand(pVar);
	}
}

spvgentwo::List<spvgentwo::Instruction*> spvgentwo::Function::getGlobalVariableInterface() const
{
	List<Instruction*> vars(m_pAllocator);

	for(BasicBlock& bb : *this)
	{
		for(Instruction& instr : bb)
		{
			// find valid OpVariable (resultType, resultId, Storage Class)
			if(instr.getOperation() == spv::Op::OpVariable && instr.size() >= 3)
			{
				// get StorageClass operand
				auto itStorageOperand = instr.begin() + 2u;
				if(itStorageOperand != instr.end())
				{
					const spv::StorageClass storage = static_cast<spv::StorageClass>(itStorageOperand->getLiteral().value);

					// uniquely add the instruction to the interface list
					if(storage != spv::StorageClass::Function && vars.contains(&instr) == false)
					{
						vars.emplace_back(&instr);
					}
				}
			}
		}
	}

	return vars;
}