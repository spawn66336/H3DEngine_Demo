#include "stage_post_processor.h"
#include "middle_layer_body.h"

StagePostProcessor::StagePostProcessor(Stage *pStage)
{
	m_pStage = pStage;
	m_dof_type = DOF_HIGH;
	m_aa_type = AA_HIGH;
	m_lod = LL_ZERO;
	m_surpresse_dof = false;
}

StagePostProcessor::~StagePostProcessor()
{
	m_pStage = 0;
}

void StagePostProcessor::configDOF(DOFType t)
{
	if(m_dof_type == t)
		return;

	// ȡ����Ӧ�ĺ�����Ϣ
	innerRemovePostProcess("ue3_uber.xml");
	innerRemovePostProcess("ue3_uber_1.xml");
	innerRemovePostProcess("ue3_uber_2.xml");

	m_dof_type = t;
}

void StagePostProcessor::configAA(AAType t)
{
	if(m_aa_type == t)
		return;

	// ȡ����Ӧ�ĺ�����Ϣ
	innerRemovePostProcess("fxaa.xml");
	innerRemovePostProcess("fxaa_1.xml");

	m_aa_type = t;
}

void StagePostProcessor::surpresseDOF(bool surpresse)
{
	if(surpresse == m_surpresse_dof)
		return;

	m_surpresse_dof = surpresse;
}

void StagePostProcessor::updatePostProcess(CCameraEntity* camera, StageAnimationPlayer* aniplayer, bool particalviewport /* = false */)
{
	if(!camera)
	{
		removeAllPostProcess(); //ɾ����������
		updatePostProcess();
		return;
	}
	const CCameraEntity::PostprocessCollection& process_collection = camera->get_postprocess_param();

	updatePostProcess(process_collection, aniplayer, particalviewport);
}

void StagePostProcessor::updatePostProcess(const CCameraEntity::PostprocessCollection& process_collection, StageAnimationPlayer* aniplayer, bool particalviewport /* = false */)
{
	if(process_collection.empty())
		removeAllPostProcess();
	else
	{
		CCameraEntity::PostprocessCollection::const_iterator coll_iter = process_collection.begin();
		for(;coll_iter != process_collection.end(); ++ coll_iter)
		{
			std::string name = "";
			name += coll_iter->first;
			name += ".xml";

			addPostProcess(particalviewport, name.c_str());//����ļ�
		}
	}

	updatePostProcess();

	if(!process_collection.empty())
	{
		CCameraEntity::PostprocessCollection::const_iterator coll_iter = process_collection.begin();
		for(;coll_iter != process_collection.end(); ++ coll_iter)
		{
			std::string name = "";
			name += coll_iter->first;
			name += ".xml";

			if(particalviewport && !canSetPostProcessPara(name.c_str()))
				continue;

			//���������Ϣ
			std::map<std::string,H3DVec4>::const_iterator it = coll_iter->second.begin();
			while(it != coll_iter->second.end())
			{
				setPostProcessParams(it->first.c_str(),it->second.ToFloatPtr(), sizeof(H3DVec4));
				++it;
			}
		}		
	}

	if(!aniplayer->getLutTexture().empty())//���ó���ǿ������
	{
		setPostProcessParams("LUT_Texturer", aniplayer->getLutTexture().c_str(), aniplayer->getLutTexture().size() * sizeof(char));
	}

	m_pStage->getILevel()->SetShadowFadeoutEnd(100);
}

void StagePostProcessor::innerRemovePostProcess(const char* postprocessname)
{
	std::string n = postprocessname;
	std::transform(n.begin(), n.end(), n.begin(), ::tolower);
	std::set<std::string>::iterator pos = m_post_process_col.find(n);

	if(pos != m_post_process_col.end())
	{
		m_pStage->getILevel()->DelPostProcess(("../data/product/postprocess/" + n).c_str());
		m_post_process_col.erase(pos);
	}
}

bool StagePostProcessor::canSetPostProcessPara(const char* postprocessname)
{
	return  stricmp(postprocessname, "ue3_uber.xml") != 0;
}

void StagePostProcessor::addPostProcess(bool particalviewport, const char* postprocessname)
{

	std::string n = postprocessname;
	std::transform(n.begin(), n.end(), n.begin(), ::tolower);

	// �����������ͽ��п���
	if(n == "ue3_uber.xml")
	{
		if(particalviewport)
			return; // ��ʹ���κ�viewport

		if(m_dof_type == DOF_LOW)
			n = "ue3_uber_1.xml";
		else if(m_dof_type == DOF_NO)
			n = "ue3_uber_2.xml";

		// lod2�²�ʹ���κξ������ɫ��ǿ
		if(m_lod == LL_TWO)
			return;
		// lod1��ʹ��uber2
		if(m_lod == LL_ONE)
			n = "ue3_uber_2.xml";
	}
	else if(n == "fxaa.xml")
	{
		if(m_aa_type == AA_LOW)
			n = "fxaa_1.xml";
		else if(m_aa_type == AA_NO)
			return;	
	}

	m_detained_post_process_col.insert(n);
}

void StagePostProcessor::removeAllPostProcess()
{
	m_detained_post_process_col.clear();
}

void StagePostProcessor::setPostProcessParams(const char* postprocess, const void * ptr, int size)
{
	if(!m_surpresse_dof)
		m_pStage->getILevel()->SetPostProcessParam(postprocess, ptr, size);
	else
	{
		if(!stricmp(postprocess, "DOF_Range") || !stricmp(postprocess, "DOF_Inner"))
		{
			float v = 2000;
			m_pStage->getILevel()->SetPostProcessParam(postprocess, &v, sizeof(float));
		}
		else
			m_pStage->getILevel()->SetPostProcessParam(postprocess, ptr, size);
	}
}

void StagePostProcessor::updatePostProcess()
{
	if(m_detained_post_process_col.empty()) // �ⲿ�����еĺ����ȥ��
	{
		// ԭ���в��ֺ��������
		if(!m_post_process_col.empty())
		{
			m_pStage->getILevel()->DelPostProcess(0);
			m_post_process_col.clear();
		}
	}
	else // �ⲿ����˲��ֺ���
	{
		// �ҵ���Ҫ�����õ�
		std::set<std::string> newpost;
		set_difference(m_detained_post_process_col.begin(), m_detained_post_process_col.end()
			, m_post_process_col.begin(), m_post_process_col.end(), std::insert_iterator<std::set<std::string> >(newpost, newpost.begin()));

		if(!newpost.empty())
		{
			for(std::set<std::string>::iterator p = newpost.begin(); p != newpost.end(); ++p)
			{
				m_pStage->getILevel()->AddPostProcess(("../data/product/postprocess/" + *p).c_str());
				m_post_process_col.insert(*p);
			}
		}

		// �ҵ���Ҫ���ٵ�
		std::set<std::string> removedpost;
		set_difference(m_post_process_col.begin(), m_post_process_col.end()
			, m_detained_post_process_col.begin(), m_detained_post_process_col.end(), std::insert_iterator<std::set<std::string> >(removedpost, removedpost.begin()));

		if(!removedpost.empty())
		{
			for(std::set<std::string>::iterator p = removedpost.begin(); p != removedpost.end(); ++p)
			{
				m_pStage->getILevel()->DelPostProcess(("../data/product/postprocess/" + *p).c_str());
			}
		}

		// �������õļ�¼
		m_post_process_col = m_detained_post_process_col;
	}

	m_detained_post_process_col.clear();
}

void StagePostProcessor::switchToLod(int lod)
{
	m_lod = lod;

	removeAllPostProcess();
	updatePostProcess();

	// ���ö�Ӧ�ĺ�����������������úú󣬻���к����ĸı�
	/*H3DI::IRender* r = m_pStage->getActorMng()->get_graphic_res_mng()->get_render_system()->get_IRender();
	if(m_lod == LL_ZERO)
	{
		r->OnCommandLine("SetEngineConfig usePostProcess true");
	}
	else if(m_lod == LL_ONE)
	{
		r->OnCommandLine("SetEngineConfig usePostProcess true");
	}
	else if(m_lod == LL_TWO)
	{
		r->OnCommandLine("SetEngineConfig usePostProcess false");
	}*/
}